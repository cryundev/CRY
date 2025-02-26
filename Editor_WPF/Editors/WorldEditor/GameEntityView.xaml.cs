using System.Windows.Controls;


namespace Editor_WPF.Editors;


//---------------------------------------------------------------------------------------------------------------------
/// GameEntityView
//---------------------------------------------------------------------------------------------------------------------
public partial class GameEntityView : UserControl
{
    public static GameEntityView Instance { get; private set; }

    //-----------------------------------------------------------------------------------------------------------------
    /// GameEntityView
    //-----------------------------------------------------------------------------------------------------------------
    public GameEntityView()
    {
        InitializeComponent();

        DataContext = null;
        Instance    = this;
    }
}