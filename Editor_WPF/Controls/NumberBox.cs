using System.Configuration;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;


namespace Editor_WPF.Controls;


//---------------------------------------------------------------------------------------------------------------------
/// NumberBox
//---------------------------------------------------------------------------------------------------------------------
[TemplatePart( Name = "PART_textBlock", Type = typeof( TextBlock ) )]
[TemplatePart( Name = "PART_TextBox",   Type = typeof( TextBox   ) )]
public class NumberBox : Control
{
    private double _originalValue;
    private double _mouseXStart;
    private double _multiplier   = 1;
    private bool   _captured     = false;
    private bool   _valueChanged = false;
    
    public double Multiplier
    {
        get => (double)GetValue( MultiplierProperty );
        set => SetValue( MultiplierProperty, value );
    }
    
    public static readonly DependencyProperty MultiplierProperty = DependencyProperty.Register( nameof( Multiplier ), typeof( double ), typeof( NumberBox ),
        new PropertyMetadata( 1.0 ) );

    public string Value
    {
        get => (string)GetValue( ValueProperty );
        set => SetValue( ValueProperty, value );
    }

    public static readonly DependencyProperty ValueProperty = DependencyProperty.Register( nameof( Value ), typeof( string ), typeof( NumberBox ),
        new FrameworkPropertyMetadata( null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault ) );

    //-----------------------------------------------------------------------------------------------------------------
    /// OnApplyTemplate
    //-----------------------------------------------------------------------------------------------------------------
    public override void OnApplyTemplate()
    {
        base.OnApplyTemplate();

        if ( GetTemplateChild( "PART_textBlock" ) is TextBlock textBlock )
        {
            textBlock.MouseLeftButtonDown += OnTextBlockMouseLeftButtonDown;
            textBlock.MouseLeftButtonUp   += OnTextBlockMouseLeftButtonUp;
            textBlock.MouseMove           += OnTextBlockMouseMove;
            textBlock.MouseWheel          += OnTextBlockMouseWheel;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnTextBlockMouseLeftButtonDown
    //-----------------------------------------------------------------------------------------------------------------
    private void OnTextBlockMouseLeftButtonDown( object sender, MouseButtonEventArgs e )
    {
        double.TryParse( Value, out _originalValue );

        Mouse.Capture( sender as UIElement );
        _captured = true;
        _valueChanged = false;
        e.Handled = true;
        
        _mouseXStart = e.GetPosition( this ).X;

        Focus();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnTextBlockMouseLeftButtonUp
    //-----------------------------------------------------------------------------------------------------------------
    private void OnTextBlockMouseLeftButtonUp( object sender, MouseButtonEventArgs e )
    {
        if ( _captured )
        {
            Mouse.Capture( null );
            _captured = false;
            e.Handled = true;

            if ( !_valueChanged && GetTemplateChild( "PART_textBox" ) is TextBox textBox )
            {
                textBox.Visibility = Visibility.Visible;
                textBox.Focus();
                textBox.SelectAll();
            }
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnTextBlockMouseMove
    //-----------------------------------------------------------------------------------------------------------------
    private void OnTextBlockMouseMove( object sender, MouseEventArgs e )
    {
        if ( _captured )
        {
            double mouseX = e.GetPosition( this ).X;
            double dist = mouseX - _mouseXStart;

            if ( Math.Abs( dist ) > SystemParameters.MinimumHorizontalDragDistance )
            {
                double newValue = _originalValue + ( dist * _multiplier * Multiplier );
                Value = newValue.ToString( "0.######" );
                _valueChanged = true;
            }
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnTextBlockMouseWheel
    //-----------------------------------------------------------------------------------------------------------------
    private void OnTextBlockMouseWheel( object sender, MouseWheelEventArgs e )
    {
        if ( _captured )
        {
            double deltaMultiplier = 0.0001;

                 if ( Keyboard.Modifiers.HasFlag( ModifierKeys.Control ) ) deltaMultiplier = 0.001;
            else if ( Keyboard.Modifiers.HasFlag( ModifierKeys.Shift   ) ) deltaMultiplier = 0.00005;
            
            _multiplier += e.Delta * deltaMultiplier;
            _multiplier = double.Clamp( _multiplier, 0.001, 1000 );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// NumberBox
    //-----------------------------------------------------------------------------------------------------------------
    static NumberBox()
    {
        DefaultStyleKeyProperty.OverrideMetadata( typeof( NumberBox ), new FrameworkPropertyMetadata( typeof( NumberBox ) ) );
    }
}