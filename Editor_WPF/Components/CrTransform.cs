using System.Numerics;
using System.Runtime.Serialization;
using Editor_WPF.Objects;
using Editor_WPF.Utilities;


namespace Editor_WPF.Components;


//---------------------------------------------------------------------------------------------------------------------
/// Transform
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
class CrTransform : CrComponent
{
    private Vector3 _positionn;
    [DataMember] public Vector3 Position
    {
        get => _positionn;
        set
        {
            if ( _positionn == value )
            {
                return;
            }

            _positionn = value;
            OnPropertyChanged( nameof( Position ) );
        }
    }
    
    private Vector3 _rotation;
    [DataMember] public Vector3 Rotation
    {
        get => _rotation;
        set
        {
            if ( _rotation == value )
            {
                return;
            }

            _rotation = value;
            OnPropertyChanged( nameof( Rotation ) );
        }
    }
    
    private Vector3 _scale;
    [DataMember] public Vector3 Scale
    {
        get => _scale;
        set
        {
            if ( _scale == value )
            {
                return;
            }

            _scale = value;
            OnPropertyChanged( nameof( Scale ) );
        }
    }

    public override IMultiSelectionComponent GetMultiSelectionComponent( MultiSelectionActor multiSelectionActor )
        => new MultiSelectionTransform( multiSelectionActor );

    //-----------------------------------------------------------------------------------------------------------------
    /// CrTransform
    //-----------------------------------------------------------------------------------------------------------------
    public CrTransform( CrObject? owner ) : base( owner )
    {
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// MultiSelectionTransform
//---------------------------------------------------------------------------------------------------------------------
sealed class MultiSelectionTransform : MultiSelectionComponent< CrTransform >
{
    private float? _positionX;
    public float? PositionX
    {
        get => _positionX;
        set
        {
            if ( _positionX.IsTheSameAs( value ) ) return;

            _positionX = value;
            OnPropertyChanged( nameof( PositionX ) );
        }
    }
    
    private float? _positionY;
    public float? PositionY
    {
        get => _positionY;
        set
        {
            if ( _positionY.IsTheSameAs( value ) ) return;

            _positionY = value;
            OnPropertyChanged( nameof( PositionY ) );
        }
    }
    
    private float? _positionZ;
    public float? PositionZ
    {
        get => _positionZ;
        set
        {
            if ( _positionZ.IsTheSameAs( value ) ) return;

            _positionZ = value;
            OnPropertyChanged( nameof( PositionZ ) );
        }
    }

    private float? _rotationX;
    public float? RotationX
    {
        get => _rotationX;
        set
        {
            if ( _rotationX.IsTheSameAs( value ) ) return;

            _rotationX = value;
            OnPropertyChanged( nameof( RotationX ) );
        }
    }
    
    private float? _rotationY;
    public float? RotationY
    {
        get => _rotationY;
        set
        {
            if ( _rotationY.IsTheSameAs( value ) ) return;

            _rotationY = value;
            OnPropertyChanged( nameof( RotationY ) );
        }
    }
    
    private float? _rotationZ;
    public float? RotationZ
    {
        get => _rotationZ;
        set
        {
            if ( _rotationZ.IsTheSameAs( value ) ) return;

            _rotationZ = value;
            OnPropertyChanged( nameof( RotationZ ) );
        }
    }
    
    private float? _scaleX;
    public float? ScaleX
    {
        get => _scaleX;
        set
        {
            if ( _scaleX.IsTheSameAs( value ) ) return;

            _scaleX = value;
            OnPropertyChanged( nameof( ScaleX ) );
        }
    }
    
    private float? _scaleY;
    public float? ScaleY
    {
        get => _scaleY;
        set
        {
            if ( _scaleY.IsTheSameAs( value ) ) return;

            _scaleY = value;
            OnPropertyChanged( nameof( ScaleY ) );
        }
    }
    
    private float? _scaleZ;
    public float? ScaleZ
    {
        get => _scaleZ;
        set
        {
            if ( _scaleZ.IsTheSameAs( value ) ) return;

            _scaleZ = value;
            OnPropertyChanged( nameof( ScaleZ ) );
        }
    }


    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateComponents
    //-----------------------------------------------------------------------------------------------------------------
    protected override bool UpdateComponents( string propertyName )
    {
        switch ( propertyName )
        {
            case nameof( PositionX ):
            case nameof( PositionY ):
            case nameof( PositionZ ):
            {
                SelectedComponents.ForEach
                (
                    c => c.Position = new Vector3
                    (
                        _positionX ?? c.Position.X, _positionY ?? c.Position.Y, _positionZ ?? c.Position.Z
                    )
                );
                return true;
            }
            
            case nameof( RotationX ):
            case nameof( RotationY ):
            case nameof( RotationZ ):
            {
                SelectedComponents.ForEach
                (
                    c => c.Rotation = new Vector3
                    (
                        _rotationX ?? c.Rotation.X, _rotationY ?? c.Rotation.Y, _rotationZ ?? c.Rotation.Z
                    )
                );
                return true;
            }
            
            case nameof( ScaleX ):
            case nameof( ScaleY ):
            case nameof( ScaleZ ):
            {
                SelectedComponents.ForEach
                (
                    c => c.Scale = new Vector3
                    (
                        _scaleX ?? c.Scale.X, _scaleY ?? c.Scale.Y, _scaleZ ?? c.Scale.Z
                    )
                );
                return true;
            }
        }

        return false;
    }
    
    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateMultiSelectionComponent
    //-----------------------------------------------------------------------------------------------------------------
    protected override bool UpdateMultiSelectionComponent()
    {
        PositionX = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Position.X ) );
        PositionY = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Position.Y ) );
        PositionZ = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Position.Z ) );
        
        RotationX = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Rotation.X ) );
        RotationY = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Rotation.Y ) );
        RotationZ = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Rotation.Z ) );
        
        ScaleX = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Scale.X ) );
        ScaleY = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Scale.Y ) );
        ScaleZ = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Scale.Z ) );

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// MultiSelectionTransform
    //-----------------------------------------------------------------------------------------------------------------
    public MultiSelectionTransform( MultiSelectionActor multiSelectionActor ) : base( multiSelectionActor )
    {
        Refresh();
    }
}