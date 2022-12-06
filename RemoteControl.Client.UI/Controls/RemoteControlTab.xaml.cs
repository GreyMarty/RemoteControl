using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
namespace RemoteControl.Client.UI.Controls
{
    /// <summary>
    /// Interaction logic for RemoteControlTab.xaml
    /// </summary>
    public partial class RemoteControlTab : UserControl, INotifyPropertyChanged
    {
        private string _connectionString = "- - - - ";
        private bool _isConnected = false;


        public string ConnectionString
        {
            get => _connectionString;
            set 
            {
                _connectionString = value;
                NotifyPropertyChanged(nameof(ConnectionString));
            }
        }

        public bool IsConnected 
        {
            get => _isConnected;
            set 
            {
                _isConnected = value;
                NotifyPropertyChanged(nameof(IsConnected));
            }
        }

        public delegate void ConnectClickHandler(object sender, string connectionString);
        public event ConnectClickHandler ConnectClick;

        public event PropertyChangedEventHandler? PropertyChanged;

        public RemoteControlTab()
        {
            InitializeComponent();
            DataContext = this;
        }

        private void ButtonConnect_Click(object sender, RoutedEventArgs e)
        {
            ConnectClick?.Invoke(this, ConnectionStringTextBox.Text);
        }

        private void NotifyPropertyChanged(string name) 
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
    }
}
