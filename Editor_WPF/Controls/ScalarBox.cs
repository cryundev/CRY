using System.Windows;


namespace Editor_WPF.Controls;


//---------------------------------------------------------------------------------------------------------------------
/// ScalarBox
//---------------------------------------------------------------------------------------------------------------------
public class ScalarBox : NumberBox
{
    static ScalarBox()
    {
        DefaultStyleKeyProperty.OverrideMetadata( typeof( ScalarBox ), new FrameworkPropertyMetadata( typeof( ScalarBox ) ) );
    }

}