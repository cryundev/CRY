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
    private float? _posX;
    public float? PosX
    {
        get => _posX;
        set
        {
            if ( _posX.IsTheSameAs( value ) ) return;

            _posX = value;
            OnPropertyChanged( nameof( PosX ) );
        }
    }
    
    private float? _posY;
    public float? PosY
    {
        get => _posY;
        set
        {
            if ( _posY.IsTheSameAs( value ) ) return;

            _posY = value;
            OnPropertyChanged( nameof( PosY ) );
        }
    }
    
    private float? _posZ;
    public float? PosZ
    {
        get => _posZ;
        set
        {
            if ( _posZ.IsTheSameAs( value ) ) return;

            _posZ = value;
            OnPropertyChanged( nameof( PosZ ) );
        }
    }

    private float? _rotX;
    public float? RotX
    {
        get => _rotX;
        set
        {
            if ( _rotX.IsTheSameAs( value ) ) return;

            _rotX = value;
            OnPropertyChanged( nameof( RotX ) );
        }
    }
    
    private float? _rotY;
    public float? RotY
    {
        get => _rotY;
        set
        {
            if ( _rotY.IsTheSameAs( value ) ) return;

            _rotY = value;
            OnPropertyChanged( nameof( RotY ) );
        }
    }
    
    private float? _rotZ;
    public float? RotZ
    {
        get => _rotZ;
        set
        {
            if ( _rotZ.IsTheSameAs( value ) ) return;

            _rotZ = value;
            OnPropertyChanged( nameof( RotZ ) );
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
            case nameof( PosX ):
            case nameof( PosY ):
            case nameof( PosZ ):
            {
                SelectedComponents.ForEach
                (
                    c => c.Position = new Vector3
                    (
                        _posX ?? c.Position.X, _posY ?? c.Position.Y, _posZ ?? c.Position.Z
                    )
                );
                return true;
            }
            
            case nameof( RotX ):
            case nameof( RotY ):
            case nameof( RotZ ):
            {
                SelectedComponents.ForEach
                (
                    c => c.Rotation = new Vector3
                    (
                        _rotX ?? c.Rotation.X, _rotY ?? c.Rotation.Y, _rotZ ?? c.Rotation.Z
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
        PosX = MultiSelectionActor.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Position.X ) );
        PosY = MultiSelectionActor.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Position.Y ) );
        PosZ = MultiSelectionActor.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Position.Z ) );
        
        RotX = MultiSelectionActor.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Rotation.X ) );
        RotY = MultiSelectionActor.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Rotation.Y ) );
        RotZ = MultiSelectionActor.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Rotation.Z ) );
        
        ScaleX = MultiSelectionActor.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Scale.X ) );
        ScaleY = MultiSelectionActor.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Scale.Y ) );
        ScaleZ = MultiSelectionActor.GetMixedValue( SelectedComponents, new Func< CrTransform, float >( x => x.Scale.Z ) );

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