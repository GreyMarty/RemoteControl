using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RemoteControl.Client.UI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private ClientController _controller;
        private int _selectedTabIndex = 0;

        public string ConnectionString => _controller?.ConnectionString;

        public int SelectedTabIndex 
        {
            get => _selectedTabIndex;
            set 
            {
                _selectedTabIndex = value;
                NotifyPropertyChanged(nameof(SelectedTabIndex));
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;


        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;

            _controller = new ClientController();

            TabList.PropertyChanged += TabList_PropertyChanged;
        }

        private void NotifyPropertyChanged(string name) 
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        private async void Window_Loaded(object sender, RoutedEventArgs e)
        {
            await _controller.ConnectAsync();
            RemoteControlTab.ConnectionString = _controller.ConnectionString ?? "Not Connected";
        }

        private async void RemoteControlTab_ConnectClick(object sender, string connectionString)
        {
            if (!_controller.IsControlling)
            {
                await _controller.TakeControlAsync(connectionString);
            }
            else 
            {
                await _controller.ForsakeControlAsync();
            }

            RemoteControlTab.IsConnected = _controller.IsControlling;
        }

        private void TabList_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "SelectedIndex") 
            {
                SelectedTabIndex = TabList.SelectedIndex;
            }
        }
    }
}
