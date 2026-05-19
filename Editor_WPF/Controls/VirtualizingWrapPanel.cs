using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Media;


namespace Editor_WPF.Controls;


//---------------------------------------------------------------------------------------------------------------------
/// VirtualizingWrapPanel
//---------------------------------------------------------------------------------------------------------------------
public sealed class VirtualizingWrapPanel : VirtualizingPanel, IScrollInfo
{
    public static readonly DependencyProperty ItemWidthProperty = DependencyProperty.Register(
        nameof( ItemWidth ),
        typeof( double ),
        typeof( VirtualizingWrapPanel ),
        new FrameworkPropertyMetadata( 120.0, FrameworkPropertyMetadataOptions.AffectsMeasure ) );

    public static readonly DependencyProperty ItemHeightProperty = DependencyProperty.Register(
        nameof( ItemHeight ),
        typeof( double ),
        typeof( VirtualizingWrapPanel ),
        new FrameworkPropertyMetadata( 106.0, FrameworkPropertyMetadataOptions.AffectsMeasure ) );

    private Size _extent;
    private Size _viewport;
    private Point _offset;

    public double ItemWidth
    {
        get => ( double )GetValue( ItemWidthProperty );
        set => SetValue( ItemWidthProperty, value );
    }

    public double ItemHeight
    {
        get => ( double )GetValue( ItemHeightProperty );
        set => SetValue( ItemHeightProperty, value );
    }

    public bool CanHorizontallyScroll { get; set; }
    public bool CanVerticallyScroll   { get; set; }

    public double ExtentWidth      => _extent.Width;
    public double ExtentHeight     => _extent.Height;
    public double ViewportWidth    => _viewport.Width;
    public double ViewportHeight   => _viewport.Height;
    public double HorizontalOffset => _offset.X;
    public double VerticalOffset   => _offset.Y;

    public ScrollViewer? ScrollOwner { get; set; }

    //-----------------------------------------------------------------------------------------------------------------
    /// MeasureOverride
    //-----------------------------------------------------------------------------------------------------------------
    protected override Size MeasureOverride( Size availableSize )
    {
        UpdateScrollInfo( availableSize );

        int itemCount = GetItemCount();
        if ( itemCount == 0 )
        {
            RemoveInternalChildRange( 0, InternalChildren.Count );
            return availableSize;
        }

        int itemsPerRow = GetItemsPerRow( _viewport.Width );
        int firstIndex  = Math.Max( 0, ( int )( _offset.Y / ItemHeight ) * itemsPerRow );
        int lastIndex   = Math.Min( itemCount - 1, firstIndex + GetVisibleItemCount( itemsPerRow ) );

        CleanUpItems( firstIndex, lastIndex );
        RealizeItems( firstIndex, lastIndex );

        return availableSize;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// ArrangeOverride
    //-----------------------------------------------------------------------------------------------------------------
    protected override Size ArrangeOverride( Size finalSize )
    {
        int itemsPerRow = GetItemsPerRow( finalSize.Width );

        for ( int i = 0; i < InternalChildren.Count; ++i )
        {
            UIElement child = InternalChildren[ i ];
            int itemIndex = ItemContainerGenerator.IndexFromGeneratorPosition( new GeneratorPosition( i, 0 ) );
            if ( itemIndex < 0 ) continue;

            int row = itemIndex / itemsPerRow;
            int column = itemIndex % itemsPerRow;

            double x = column * ItemWidth  - _offset.X;
            double y = row    * ItemHeight - _offset.Y;

            child.Arrange( new Rect( x, y, ItemWidth, ItemHeight ) );
        }

        return finalSize;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// LineUp
    //-----------------------------------------------------------------------------------------------------------------
    public void LineUp()
    {
        SetVerticalOffset( VerticalOffset - ItemHeight );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// LineDown
    //-----------------------------------------------------------------------------------------------------------------
    public void LineDown()
    {
        SetVerticalOffset( VerticalOffset + ItemHeight );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// LineLeft
    //-----------------------------------------------------------------------------------------------------------------
    public void LineLeft()
    {
        SetHorizontalOffset( HorizontalOffset - ItemWidth );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// LineRight
    //-----------------------------------------------------------------------------------------------------------------
    public void LineRight()
    {
        SetHorizontalOffset( HorizontalOffset + ItemWidth );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// MouseWheelUp
    //-----------------------------------------------------------------------------------------------------------------
    public void MouseWheelUp()
    {
        SetVerticalOffset( VerticalOffset - ItemHeight * 3 );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// MouseWheelDown
    //-----------------------------------------------------------------------------------------------------------------
    public void MouseWheelDown()
    {
        SetVerticalOffset( VerticalOffset + ItemHeight * 3 );
    }

    public void MouseWheelLeft()  => LineLeft();
    public void MouseWheelRight() => LineRight();
    public void PageUp()          => SetVerticalOffset( VerticalOffset - ViewportHeight );
    public void PageDown()        => SetVerticalOffset( VerticalOffset + ViewportHeight );
    public void PageLeft()        => SetHorizontalOffset( HorizontalOffset - ViewportWidth );
    public void PageRight()       => SetHorizontalOffset( HorizontalOffset + ViewportWidth );

    //-----------------------------------------------------------------------------------------------------------------
    /// SetHorizontalOffset
    //-----------------------------------------------------------------------------------------------------------------
    public void SetHorizontalOffset( double offset )
    {
        double maxOffset = Math.Max( 0, ExtentWidth - ViewportWidth );
        _offset.X = Math.Clamp( offset, 0, maxOffset );

        InvalidateMeasure();
        ScrollOwner?.InvalidateScrollInfo();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// SetVerticalOffset
    //-----------------------------------------------------------------------------------------------------------------
    public void SetVerticalOffset( double offset )
    {
        double maxOffset = Math.Max( 0, ExtentHeight - ViewportHeight );
        _offset.Y = Math.Clamp( offset, 0, maxOffset );

        InvalidateMeasure();
        ScrollOwner?.InvalidateScrollInfo();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// MakeVisible
    //-----------------------------------------------------------------------------------------------------------------
    public Rect MakeVisible( Visual visual, Rect rectangle )
    {
        return rectangle;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateScrollInfo
    //-----------------------------------------------------------------------------------------------------------------
    private void UpdateScrollInfo( Size availableSize )
    {
        double viewportWidth  = double.IsInfinity( availableSize.Width  ) ? 0 : availableSize.Width;
        double viewportHeight = double.IsInfinity( availableSize.Height ) ? 0 : availableSize.Height;

        int itemsPerRow = GetItemsPerRow( viewportWidth );
        int rowCount = ( int )Math.Ceiling( GetItemCount() / ( double )itemsPerRow );

        Size extent = new Size( viewportWidth, rowCount * ItemHeight );
        Size viewport = new Size( viewportWidth, viewportHeight );

        if ( extent != _extent || viewport != _viewport )
        {
            _extent = extent;
            _viewport = viewport;
            ScrollOwner?.InvalidateScrollInfo();
        }

        _offset.X = Math.Clamp( _offset.X, 0, Math.Max( 0, ExtentWidth  - ViewportWidth  ) );
        _offset.Y = Math.Clamp( _offset.Y, 0, Math.Max( 0, ExtentHeight - ViewportHeight ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetVisibleItemCount
    //-----------------------------------------------------------------------------------------------------------------
    private int GetVisibleItemCount( int itemsPerRow )
    {
        int visibleRowCount = ( int )Math.Ceiling( ViewportHeight / ItemHeight ) + 2;
        return Math.Max( itemsPerRow, visibleRowCount * itemsPerRow );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetItemsPerRow
    //-----------------------------------------------------------------------------------------------------------------
    private int GetItemsPerRow( double width )
    {
        if ( width <= 0 || ItemWidth <= 0 ) return 1;

        return Math.Max( 1, ( int )( width / ItemWidth ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetItemCount
    //-----------------------------------------------------------------------------------------------------------------
    private int GetItemCount()
    {
        return ItemsControl.GetItemsOwner( this )?.Items.Count ?? 0;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// RealizeItems
    //-----------------------------------------------------------------------------------------------------------------
    private void RealizeItems( int firstIndex, int lastIndex )
    {
        GeneratorPosition startPosition = ItemContainerGenerator.GeneratorPositionFromIndex( firstIndex );
        int childIndex = startPosition.Offset == 0 ? startPosition.Index : startPosition.Index + 1;

        using IDisposable generator = ItemContainerGenerator.StartAt( startPosition, GeneratorDirection.Forward, true );

        for ( int itemIndex = firstIndex; itemIndex <= lastIndex; ++itemIndex, ++childIndex )
        {
            bool isNewlyRealized;
            UIElement child = ( UIElement )ItemContainerGenerator.GenerateNext( out isNewlyRealized );

            if ( isNewlyRealized )
            {
                if ( childIndex >= InternalChildren.Count )
                {
                    AddInternalChild( child );
                }
                else
                {
                    InsertInternalChild( childIndex, child );
                }

                ItemContainerGenerator.PrepareItemContainer( child );
            }

            child.Measure( new Size( ItemWidth, ItemHeight ) );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// CleanUpItems
    //-----------------------------------------------------------------------------------------------------------------
    private void CleanUpItems( int firstIndex, int lastIndex )
    {
        for ( int i = InternalChildren.Count - 1; i >= 0; --i )
        {
            GeneratorPosition generatorPosition = new GeneratorPosition( i, 0 );
            int itemIndex = ItemContainerGenerator.IndexFromGeneratorPosition( generatorPosition );
            if ( itemIndex >= firstIndex && itemIndex <= lastIndex ) continue;

            ItemContainerGenerator.Remove( generatorPosition, 1 );
            RemoveInternalChildRange( i, 1 );
        }
    }
}
