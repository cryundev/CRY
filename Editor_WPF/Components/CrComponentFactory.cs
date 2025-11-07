using System.Diagnostics;
using Editor_WPF.Objects;


namespace Editor_WPF.Components;


//---------------------------------------------------------------------------------------------------------------------
/// ComponentType
//---------------------------------------------------------------------------------------------------------------------
public enum ComponentType
{
    Transform,
    Script,
}

//---------------------------------------------------------------------------------------------------------------------
/// CrComponentFactory
//---------------------------------------------------------------------------------------------------------------------
public class CrComponentFactory
{
    private static readonly Func< CrActor, object, CrComponent >[] _function = new Func< CrActor, object, CrComponent >[]
    {
        ( actor, data ) => new CrTransform( actor ),
        ( actor, data ) => new CrScript   ( actor ) { Name = (string)data },
    };

    public static Func< CrActor, object, CrComponent > GetCreationFunction( ComponentType componentType )
    {
        Debug.Assert( (int)componentType < _function.Length );

        return _function[ (int)componentType ];
    }
}