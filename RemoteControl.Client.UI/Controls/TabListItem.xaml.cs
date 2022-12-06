using System;
using System.Collections.Generic;
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
    /// Interaction logic for TabListItem.xaml
    /// </summary>
    public partial class TabListItem : UserControl
    {
        public static readonly DependencyProperty IconDataProperty = DependencyProperty.Register(nameof(IconData), typeof(Geometry), typeof(TabListItem));
        public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(nameof(Title), typeof(string), typeof(TabListItem));


        public Geometry IconData
        {
            get => (Geometry)GetValue(IconDataProperty);
            set => SetValue(IconDataProperty, value);
        }

        public string Title 
        {
            get => (string)GetValue(TitleProperty);
            set => SetValue(TitleProperty, value);
        } 


        public TabListItem()
        {
            InitializeComponent();
            DataContext = this;
        }
    }
}
