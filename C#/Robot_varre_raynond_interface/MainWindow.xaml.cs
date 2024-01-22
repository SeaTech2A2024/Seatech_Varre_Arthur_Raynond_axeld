using ExtendedSerialPort;
using System.Windows.Threading;
using System;
using System.Collections.Generic;
using System.IO.Ports;
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

namespace Robot_varre_raynond_interface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        string ReceivedText;
        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM21", 115200, Parity.None, 8, StopBits.One);
            serialPort1.OnDataReceivedEvent += SerialPort1_OnDataReceivedEvent;
            //serialPort1.Open(); 
            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();
        }

        private void TimerAffichage_Tick(object? sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        private void SerialPort1_OnDataReceivedEvent(object? sender, DataReceivedArgs e)
        {
            ReceivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);

        }

        private void TextBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                Sendmessage();
            }
        }

        private void TextBoxReception_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void Sendmessage()
        {
            string message = TextBoxEmission.Text;
            serialPort1.WriteLine(message);
            TextBoxEmission.Text = null;
        }

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            if(buttonEnvoyer.Background == Brushes.Beige)
            {
                buttonEnvoyer.Background = Brushes.LightGray;
            }
            else
            {
                buttonEnvoyer.Background = Brushes.Beige;
            }
            Sendmessage();
        }

  

    }
}
