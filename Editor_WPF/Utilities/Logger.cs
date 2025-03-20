using System.Collections.ObjectModel;
using System.IO;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Data;

namespace Editor_WPF.Utilities;


//---------------------------------------------------------------------------------------------------------------------
/// MessageType
//---------------------------------------------------------------------------------------------------------------------
public enum MessageType
{
    Info    = 0x01,
    Warning = 0x02,
    Error   = 0x04
};


//---------------------------------------------------------------------------------------------------------------------
/// LogMessage
//---------------------------------------------------------------------------------------------------------------------
public class LogMessage
{
    public DateTime    Time        { get; }
    public MessageType MessageType { get; }
    public string      Message     { get; }        
    public string      File        { get; }        
    public string      Caller      { get; }
    public int         Line        { get; }
    public string      MetaDta     => $"{File} : {Caller} ({Line})";


    //-----------------------------------------------------------------------------------------------------------------
    /// LogMessage
    //-----------------------------------------------------------------------------------------------------------------
    public LogMessage( MessageType messageType, string message, string file, string caller, int line )
    {
        Time        = DateTime.Now;
        MessageType = messageType;
        Message     = message;
        File        = Path.GetFileName( file );
        Caller      = caller;
        Line        = line;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Logger
//---------------------------------------------------------------------------------------------------------------------
static class Logger
{
    private static int _messageFilter = (int)( MessageType.Info | MessageType.Warning | MessageType.Error );
    
    private static ObservableCollection< LogMessage > _messages = new ObservableCollection< LogMessage >();
    public static ReadOnlyObservableCollection< LogMessage > Messages { get; } = new ReadOnlyObservableCollection< LogMessage >( _messages );

    public static CollectionViewSource FilteredMessages { get; } = new CollectionViewSource() { Source = Messages };


    //-----------------------------------------------------------------------------------------------------------------
    /// Log
    //-----------------------------------------------------------------------------------------------------------------
    public static async void Log( MessageType type, string msg, [CallerFilePath] string file = "",
        [CallerMemberName] string caller = "", [CallerLineNumber] int line = 0 )
    {
        await Application.Current.Dispatcher.BeginInvoke( new Action( () =>
        {
            _messages.Add( new LogMessage( type, msg, file, caller, line ) );
        } ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Clear
    //-----------------------------------------------------------------------------------------------------------------
    public static async void Clear()
    {
        await Application.Current.Dispatcher.BeginInvoke( new Action( () =>
        {
            _messages.Clear();
        } ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// SetMessageFilter
    //-----------------------------------------------------------------------------------------------------------------
    public static void SetMessageFilter( int mask )
    {
        _messageFilter = mask;
        FilteredMessages.View.Refresh();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Logger
    //-----------------------------------------------------------------------------------------------------------------
    static Logger()
    {
        FilteredMessages.Filter += ( s, e ) =>
        {
            int type = (int)( e.Item as LogMessage ).MessageType;
            e.Accepted = ( type & _messageFilter ) != 0;
        };
    }
}