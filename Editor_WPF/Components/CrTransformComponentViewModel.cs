using System.Numerics;
using System.Runtime.Serialization;
using Editor_WPF.DllWrappers;
using Editor_WPF.Objects;
using Editor_WPF.Utilities;


namespace Editor_WPF.Components;


//---------------------------------------------------------------------------------------------------------------------
/// Transform
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
class CrTransformComponentViewModel : CrComponentViewModel
{
    private Vector3 _position;
    [DataMember] public Vector3 Position
    {
        get => _position;
        set
        {
            if ( _position == value )
            {
                return;
            }

            _position = value;
            OnPropertyChanged( nameof( Position ) );
            SyncActorTransform();
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
            SyncActorTransform();
        }
    }
    
    private Vector3 _scale = Vector3.One;
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
            SyncActorTransform();
        }
    }

    public override IMultiSelectionComponent GetMultiSelectionComponent( MultiSelectionActor multiSelectionActor )
        => new MultiSelectionTransform( multiSelectionActor );

    //-----------------------------------------------------------------------------------------------------------------
    /// RefreshFromEngine
    //-----------------------------------------------------------------------------------------------------------------
    public bool RefreshFromEngine()
    {
        if ( Owner is not CrActorViewModel actor || !ID.IsValid( actor.ActorId ) )
        {
            return false;
        }

        if ( !EngineAPI.Actor.TryGetTransform( actor.ActorId, out EngineAPI.CrTransformNative transform ) )
        {
            return false;
        }

        bool changed = false;

        Vector3 position = transform.Position.ToVector3();
        if ( _position != position )
        {
            _position = position;
            OnPropertyChanged( nameof( Position ) );
            changed = true;
        }

        Vector3 rotation = transform.RotationEuler.ToVector3();
        if ( _rotation != rotation )
        {
            _rotation = rotation;
            OnPropertyChanged( nameof( Rotation ) );
            changed = true;
        }

        Vector3 scale = transform.Scale.ToVector3();
        if ( _scale != scale )
        {
            _scale = scale;
            OnPropertyChanged( nameof( Scale ) );
            changed = true;
        }

        return changed;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// SyncActorTransform
    //-----------------------------------------------------------------------------------------------------------------
    private void SyncActorTransform()
    {
        if ( Owner is not CrActorViewModel actor || !ID.IsValid( actor.ActorId ) ) return;

        EngineAPI.Actor.TrySetTransform( actor.ActorId, Position, Rotation, Scale );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// CrTransformComponentViewModel
    //-----------------------------------------------------------------------------------------------------------------
    public CrTransformComponentViewModel( CrObjectViewModel? owner ) : base( owner )
    {
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// MultiSelectionTransform
//---------------------------------------------------------------------------------------------------------------------
sealed class MultiSelectionTransform : MultiSelectionComponent< CrTransformComponentViewModel >
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
        PositionX = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransformComponentViewModel, float >( x => x.Position.X ) );
        PositionY = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransformComponentViewModel, float >( x => x.Position.Y ) );
        PositionZ = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransformComponentViewModel, float >( x => x.Position.Z ) );
        
        RotationX = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransformComponentViewModel, float >( x => x.Rotation.X ) );
        RotationY = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransformComponentViewModel, float >( x => x.Rotation.Y ) );
        RotationZ = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransformComponentViewModel, float >( x => x.Rotation.Z ) );
        
        ScaleX = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransformComponentViewModel, float >( x => x.Scale.X ) );
        ScaleY = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransformComponentViewModel, float >( x => x.Scale.Y ) );
        ScaleZ = UtilObject.GetMixedValue( SelectedComponents, new Func< CrTransformComponentViewModel, float >( x => x.Scale.Z ) );

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
