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

namespace RemoteControl.Client.UI.Controls
{
    /// <summary>
    /// Interaction logic for TabList.xaml
    /// </summary>
    public partial class TabList : UserControl, INotifyPropertyChanged
    {
        private int _selectedIndex = 0;


        public int SelectedIndex 
        {
            get => _selectedIndex;
            set 
            {
                _selectedIndex = value;
                NotifyPropertyChanged(nameof(SelectedIndex));
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;


        public TabList()
        {
            InitializeComponent();
            DataContext = this;
        }

        private void NotifyPropertyChanged(string name) 
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        private class TabItem 
        {
            public Geometry IconData { get; set; }
            public string Text { get; set; }
        }
    }
}
