using System.Collections.Specialized;
using System.Windows;
using System.Windows.Controls;
using Editor_WPF.GameProject;


namespace Editor_WPF.Editors;


//---------------------------------------------------------------------------------------------------------------------
/// WorldEditorView
//---------------------------------------------------------------------------------------------------------------------
public partial class WorldEditorView
{
    //-----------------------------------------------------------------------------------------------------------------
    /// WorldEditorView
    //-----------------------------------------------------------------------------------------------------------------
    public WorldEditorView()
    {
        InitializeComponent();
        
        Loaded += OnWorldEditorViewLoaded;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnWorldEditorViewLoaded
    //-----------------------------------------------------------------------------------------------------------------
    private void OnWorldEditorViewLoaded( object sender, RoutedEventArgs eventArgs )
    {
        Loaded -= OnWorldEditorViewLoaded;
        
        Focus();

        ( (INotifyCollectionChanged)Project.UndoRedo.UndoList ).CollectionChanged += ( s, e ) => Focus();
    }
}