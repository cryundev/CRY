using System.Diagnostics;
using Editor_WPF.Objects;


namespace Editor_WPF.Components;


//---------------------------------------------------------------------------------------------------------------------
/// ComponentType
//---------------------------------------------------------------------------------------------------------------------
public enum ComponentType
{
    Transform,
    Script,
}

//---------------------------------------------------------------------------------------------------------------------
/// CrComponentFactory
//---------------------------------------------------------------------------------------------------------------------
public class CrComponentFactory
{
    private static readonly Func< CrActorViewModel, object, CrComponentViewModel >[] _function = new Func< CrActorViewModel, object, CrComponentViewModel >[]
    {
        ( actor, data ) => new CrTransformComponentViewModel( actor ),
        ( actor, data ) => new CrScriptComponentViewModel( actor ) { Name = (string)data },
    };

    public static Func< CrActorViewModel, object, CrComponentViewModel > GetCreationFunction( ComponentType componentType )
    {
        Debug.Assert( (int)componentType < _function.Length );

        return _function[ (int)componentType ];
    }
}