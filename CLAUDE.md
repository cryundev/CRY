# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

CRY is a 3D game engine built with DirectX 11 for learning rendering techniques. The project consists of:

- **Engine**: Core C++ rendering engine library (static lib)
- **EngineDLL**: Engine functionality exposed as a DLL for editor integration
- **Editor_WPF**: C# WPF-based MVVM editor with project and world management
- **Editor_Win32**: Native Windows editor (legacy/test)
- **EngineTest**: Testing framework for engine components

Key integration points:
- Editor loads game code DLLs built from user projects
- Engine C++ code is wrapped and exposed via EngineDLL for editor access
- Script system uses DLL exports (`GetScriptCreator`, `GetScriptNames`) for dynamic component loading

## Build System

This is a Visual Studio solution (`CRY.sln`) using MSBuild with both C++ and C# projects.

### Build Configurations
- `Debug|x64`: Debug build for runtime
- `Release|x64`: Release build for runtime  
- `DebugEditor|x64`: Debug build including editor tools
- `ReleaseEditor|x64`: Release build including editor tools

### Build Commands
Build the entire solution:
```bash
msbuild CRY.sln /p:Configuration=DebugEditor /p:Platform=x64
```

Build specific projects:
```bash
msbuild Engine/Engine.vcxproj /p:Configuration=Debug /p:Platform=x64
msbuild Editor_WPF/Editor_WPF.csproj /p:Configuration=DebugEditor /p:Platform=x64
```

Run tests:
```bash
x64\DebugEditor\EngineTest.exe
```

The Editor_WPF project manages game code compilation using the `CodeEditorManager` which supports Visual Studio and Rider integration.

## Architecture

### Engine (C++)
Core engine structure under `Engine/Source/`:

- **Core/**: Fundamental types, math, containers, transform system
  - Custom type aliases: `i8/i16/i32/i64`, `u8/u16/u32/u64`, `f32/f64`
  - Component concepts and templates
- **Object/**: Entity-component system with template-based static storage
  - `CRActor`: Base entity class with component management (`AddComponent`, `GetComponent`, `RemoveComponent`)
  - `CRComponent<T>`: CRTP base with static component storage per type using ID mapping
  - `ICRComponent`: Interface for polymorphic component operations (initialization, update)
  - `CRScript`: Base for user-defined game logic scripts with `REGISTER_SCRIPT` macro
  - Component registration uses static arrays and ID-to-index mapping for fast lookup
  - Game object hierarchy with `CRGameInstance` and `CRGameMode`
- **RHI/**: Rendering Hardware Interface abstraction
  - `CRRHI`: Main rendering interface (currently DirectX 11)
  - DirectX 11 implementation in `RHI/DX11/`
  - Resource management for buffers, shaders, textures
- **Asset/**: Asset loading and management system
- **Utility/**: FBX loading, logging, time management, string utilities
- **World/**: Scene management with `CRWorld`

### Editor (C#/WPF)
WPF-based MVVM editor under `Editor_WPF/`:

- **GameProject/**: Project and world management with MVVM ViewModels
  - `ProjectViewModel`: Main project state, world collection, build management, DLL loading/unloading
  - `WorldViewModel`: Individual scene/world management with actor collections
  - `.cryproject` files for project serialization using `[DataContract]` attributes
  - Build configuration management (Debug/Release/DebugEditor/ReleaseEditor)
  - Undo/redo system integrated with ViewModels via `UndoRedoAction`
- **Editors/WorldEditor/**: Scene editing interface with actor/component views
- **GameDev/**: Code editor integration (Visual Studio/Rider support)
  - `CodeEditorManager`: Manages external IDE integration and build process
  - Build completion event triggers DLL reload workflow
- **Utilities/**: Undo/redo system, logging, serialization
- **DllWrappers/**: C# wrappers for Engine DLL functions (`EngineAPI`)
- **Objects/**: Editor representations of engine objects (`CrActor`, component views)
- **ProjectTemplates/**: Template projects (Empty, FirstPerson, ThirdPerson)

### External Dependencies
- **DirectXTK**: Microsoft's DirectX Toolkit for rendering utilities
- **FBX SDK**: Autodesk FBX SDK for 3D model loading
- **ImGUI**: Immediate mode GUI for debugging interfaces

## Development Workflow

1. **Project Creation**: Use Editor_WPF to create new projects from templates
2. **Code Editing**: Editor integrates with Visual Studio/Rider for C++ game code
3. **Building**: Projects build game code DLLs that get loaded by the editor
   - `ProjectViewModel.BuildGameCodeDll()` triggers MSBuild via `CodeEditorManager`
   - Build completion event (`OnBuildCompleted`) automatically triggers DLL reload
4. **DLL Loading**: Editor automatically loads/unloads game DLLs after building
   - `EngineAPI.LoadGameCodeDLL()` loads the built DLL
   - `EngineAPI.GetScriptNames()` retrieves available scripts from loaded DLL
   - Scripts can be added to actors via component factory system
5. **Asset Pipeline**: FBX models and textures loaded through engine asset system

## Key Files and Conventions

### Component System Architecture
- **C++ Side**: CRTP-based `CRComponent<T>` with static storage per component type
  - Each component type has its own static `Components` array and `IdMap` for O(1) lookup
  - Uses C++20 concepts (`ComponentType`, `ActorType`) for compile-time type safety
  - Components are accessed via actor's ObjectId mapped to component storage index
- **C# Editor Side**: ViewModels represent engine objects for MVVM binding
  - `CrActor` in editor corresponds to `CRActor` in engine (via EngineAPI wrapper)
  - Component views dynamically created based on loaded script DLL exports

### Naming Conventions
- **Engine Classes**: Prefixed with `CR` (e.g., `CRActor`, `CRComponent`)
- **Editor Classes**:
  - ViewModels suffixed with `ViewModel` (e.g., `ProjectViewModel`, `WorldViewModel`)
  - Editor object representations prefixed with `Cr` (e.g., `CrActor`, `CrScript`)
- **File Organization**:
  - `.cryproject` files store project configuration and world data
  - Build output to `x64/[Configuration]/` directories
  - Game code DLLs named `{ProjectName}.dll`

## Important Patterns

### MVVM Architecture in Editor
The WPF editor follows strict MVVM pattern:
- **ViewModels**: Data and commands exposed to Views (e.g., `ProjectViewModel`, `WorldViewModel`)
- **Commands**: `ICommand` implementations via `RelayCommand<T>` for user actions
- **Undo/Redo**: Integrated at ViewModel level using `UndoRedoAction` with undo/redo lambdas
- **Data Binding**: Properties use `OnPropertyChanged` for two-way binding
- **Serialization**: ViewModels marked with `[DataContract]` and `[DataMember]` for XML serialization

### DLL Lifecycle Management
Critical pattern for hot-reloading game code:
1. User requests build → `CodeEditorManager.BuildSolution()` called
2. Before build: `UnloadGameCodeDll()` releases existing DLL
3. Subscribe to `CodeEditorManager.BuildCompleted` event
4. On success: `LoadGameCodeDll()` loads new DLL and calls `EngineAPI.GetScriptNames()`
5. Available scripts updated in `ProjectViewModel.AvailableScripts` property

### Component Registration Pattern (C++)
User-defined scripts use macro registration:
```cpp
class MyScript : public CRScript {
    // Implementation
};
REGISTER_SCRIPT(MyScript);  // Static registration at startup
```
The `REGISTER_SCRIPT` macro creates static initialization that registers the script creator function before main() runs.

### Recent Refactoring Notes
- `World.cs` was renamed to `WorldViewModel.cs` to better reflect MVVM architecture
- All world-related logic now properly encapsulated in ViewModel pattern
- Script component system enhanced with dynamic component factory and improved script management

## External Library Integration

External libraries are included in `Engine/Extras/`:
- DirectXTK build configuration matches main project
- FBX SDK libraries and headers for model loading
- Build events copy required DLLs to output directory

## Claude Code Memory

Project analysis, coding style guide, and architecture notes are stored in `.claude/memory/`:
- **[MEMORY.md](.claude/memory/MEMORY.md)**: Quick reference for project context, conventions, known issues
- **[coding-style.md](.claude/memory/coding-style.md)**: Detailed C++/C# coding style guide with examples
- **[analysis-report.md](.claude/memory/analysis-report.md)**: Full architecture analysis and improvement roadmap