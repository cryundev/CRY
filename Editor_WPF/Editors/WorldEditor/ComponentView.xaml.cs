using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;


namespace Editor_WPF.Editors;


//---------------------------------------------------------------------------------------------------------------------
/// ComponentView
//---------------------------------------------------------------------------------------------------------------------
[ContentProperty("ComponentContent")]
public partial class ComponentView : UserControl
{
    public string Header
    {
        get => (string)GetValue( HeaderProperty );
        set => SetValue( HeaderProperty, value );
    }
    
    public static readonly DependencyProperty HeaderProperty = DependencyProperty.Register( nameof( Header ), typeof( string ), typeof( ComponentView ),
        new PropertyMetadata( "" ) );

    public FrameworkElement ComponentContent 
    {
        get => (FrameworkElement)GetValue( ComponentContentProperty );
        set => SetValue( ComponentContentProperty, value );
    }
    
    public static readonly DependencyProperty ComponentContentProperty = DependencyProperty.Register( nameof( ComponentContent ), typeof( FrameworkElement ), typeof( ComponentView ),
        new PropertyMetadata( null ) );


    //-----------------------------------------------------------------------------------------------------------------
    /// ComponentView
    //-----------------------------------------------------------------------------------------------------------------
    public ComponentView()
    {
        InitializeComponent();
    }
}