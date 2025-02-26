using System.Numerics;
using System.Runtime.Serialization;


namespace Editor_WPF.Components;


//---------------------------------------------------------------------------------------------------------------------
/// Transform
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
public class Transform : Component
{
    private Vector3 _positionn;
    
    [DataMember]
    public Vector3 Position
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
    
    [DataMember]
    public Vector3 Rotation
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
    
    [DataMember]
    public Vector3 Scale
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


    //-----------------------------------------------------------------------------------------------------------------
    /// Transform
    //-----------------------------------------------------------------------------------------------------------------
    public Transform( GameEntity owner ) : base( owner )
    {
    }
    
}