using System.Windows.Input;


namespace Editor_WPF.Common;


//---------------------------------------------------------------------------------------------------------------------
/// RelayCommand
//---------------------------------------------------------------------------------------------------------------------
public class RelayCommand< T >( Action< T > execute, Predicate< T >? canExecute = null ) : ICommand
{
    private readonly Action< T > _execute = execute ?? throw new ArgumentException( nameof( execute ) );

    public event EventHandler? CanExecuteChanged
    {
        add    => CommandManager.RequerySuggested += value;
        remove => CommandManager.RequerySuggested -= value;
    }


    //-----------------------------------------------------------------------------------------------------------------
    /// CanExecute
    //-----------------------------------------------------------------------------------------------------------------
    public bool CanExecute( object? parameter )
    {
        if ( parameter != null )
        {
            return canExecute?.Invoke( (T)parameter ) ?? true;
        }

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Execute
    //-----------------------------------------------------------------------------------------------------------------
    public void Execute( object? parameter )
    {
        _execute( ((T)parameter!)! );
    }
}