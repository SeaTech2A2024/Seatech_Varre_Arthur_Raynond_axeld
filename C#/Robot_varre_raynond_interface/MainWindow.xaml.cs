using ExtendedSerialPort_NS;
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
        ExtendedSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        Robot robot = new Robot();

        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM21", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open(); 
            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();
        }

        private void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            //robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            for(int i=0; i<e.Data.Length; i++)
            {
                robot.byteListReceived.Enqueue(e.Data[i]);
            }
        }



        private void TimerAffichage_Tick(object? sender, EventArgs e)
        {
            while(robot.byteListReceived.Count > 0)
            {
                var b = robot.byteListReceived.Dequeue();
                TextBoxReception.Text += b.ToString("X2") + " ";
            }
            //if (robot.receivedText != "")
            //{
            //    TextBoxReception.Text = robot.receivedText;
            //    robot.receivedText = "";
            //}
            //throw new NotImplementedException();
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
        private void buttonClear_Click(object sender, RoutedEventArgs e)
        {
            TextBoxReception.Text = "";
        }
        private void buttonTest_Click(object sender, RoutedEventArgs e)
        {
            byte[] byteList = new byte[20];
            for (int i = 0; i < 20; i++)
            {
                byteList[i] = (byte)(2 * i);
            }
            serialPort1.Write(byteList, 0, byteList.Length);
        }
    }
}
