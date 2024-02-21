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
using System.Net.NetworkInformation;
using System.Net.Security;

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
            for (int i = 0; i < e.Data.Length; i++)
            {
                robot.byteListReceived.Enqueue(e.Data[i]);
            }
        }



        private void TimerAffichage_Tick(object? sender, EventArgs e)
        {
            while (robot.byteListReceived.Count > 0)
            {
                byte b = robot.byteListReceived.Dequeue();
                DecodeMessage(b);

            //    TextBoxReception.Text += b.ToString("X2") + " ";
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
            if (buttonEnvoyer.Background == Brushes.Beige)
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
            //byte[] byteList = new byte[20];
            //for (int i = 0; i < 20; i++)
            //{
            //    byteList[i] = (byte)(2 * i);
            //}
            //serialPort1.Write(byteList, 0, byteList.Length);
                //string message = TextBoxEmission.Text;

                    byte[] array = Encoding.ASCII.GetBytes("Bonjour");
                    UartEncodeAndSendMessage(0x0080, array.Length, array);

                    UartEncodeAndSendMessage(0x0020, 2, new byte[] { 1,1 });
     
             
                    UartEncodeAndSendMessage(0x0030, 3, new byte[] { 40, 50,40 });
 
                    UartEncodeAndSendMessage(0x0040, 2, new byte[] { 40, 60 });
                           
            }
            byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum = 0;

            checksum ^= 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction >> 0);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength >> 0);

            for (int i = 0; i < msgPayload.Length; i++)
            {
                checksum ^= msgPayload[i];
            }
            return checksum;
        }

        void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte[] messageUART = new byte[msgPayload.Length + 6];
            messageUART[0] = 0xFE;
            messageUART[1] = (byte)(msgFunction >> 8);
            messageUART[2] = (byte)(msgFunction >> 0);
            messageUART[3] = (byte)(msgPayloadLength >> 8);
            messageUART[4] = (byte)(msgPayloadLength >> 0);

            for (int i = 0; i < msgPayload.Length; i++)
            {
                messageUART[5 + i] = msgPayload[i];
            }
            messageUART[5 + msgPayload.Length] += CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
            serialPort1.Write(messageUART, 0, 6 + msgPayload.Length);
        }

        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload;
        int msgDecodedPayloadIndex = 0;

        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                    if (c == 0xFE)
                        rcvState = StateReception.FunctionMSB;

                    break;


                case StateReception.FunctionMSB:
                    msgDecodedFunction = c << 8;
                    rcvState = StateReception.FunctionLSB;
                    break;


                case StateReception.FunctionLSB:
                    msgDecodedFunction |= c;
                    rcvState = StateReception.PayloadLengthMSB;
                    break;


                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = c << 8;
                    rcvState = StateReception.PayloadLengthLSB;
                    break;


                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength |= c << 0;
                    rcvState = StateReception.Payload;

                    if (msgDecodedPayloadLength == 0)
                        rcvState = StateReception.CheckSum;
                    else if (msgDecodedPayloadLength >= 256)
                        rcvState = StateReception.Waiting;
                    else
                    {
                        rcvState = StateReception.Payload;
                        msgDecodedPayloadIndex = 0;
                        msgDecodedPayload = new Byte[msgDecodedPayloadLength];
                    }
                    break;


                case StateReception.Payload:
                    msgDecodedPayload[msgDecodedPayloadIndex] = c;
                    msgDecodedPayloadIndex++;
                    if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                        rcvState = StateReception.CheckSum;
                    break;


                case StateReception.CheckSum:
                    byte receivedChecksum = c;
                    byte calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    if (calculatedChecksum == receivedChecksum)
                    {
                        TextBoxReception.Text+="Checksum atteint";
                    }
                    else
                    {
                        TextBoxReception.Text = "ERRRROR";
                    }
                    rcvState = StateReception.Waiting;
                    break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }

        private void LabelIRC_KeyUp(object sender, KeyEventArgs e)
        {
        }

        private void LabelIRD_KeyUp(object sender, KeyEventArgs e)
        {

        }

        private void LabelIRG_KeyUp(object sender, KeyEventArgs e)
        {

        }

        private void CheckBoxLedBlanche_KeyUp(object sender, KeyEventArgs e)
        {

        }

        private void LEDR_KeyUp(object sender, KeyEventArgs e)
        {

        }

        private void LEDV_KeyUp(object sender, KeyEventArgs e)
        {

        }

        private void VitesseGauche_KeyUp(object sender, KeyEventArgs e)
        {

        }

        private void VitesseDroite_KeyUp(object sender, KeyEventArgs e)
        {

        }

        private void CheckBoxLedRouge_KeyUp(object sender, KeyEventArgs e)
        {

        }

        private void CheckBoxLedVert_KeyUp(object sender, KeyEventArgs e)
        {

        }
    }
    public enum StateReception
    {
        Waiting,
        FunctionMSB,
        PayloadLengthMSB,
        PayloadLengthLSB,
        Payload,
        CheckSum,
        FunctionLSB
    }
    public enum CommandID
    {
        Texte=0x0080,
        LED=0x0020,
        Télémètre=0x0030,
        Consigne=0x0040
    }
    void ProcessDecodedMessage(int msgFunction,int msgPayloadLength, byte[] msgPayload)
    {
        switch (msgFunction) { }
    }

}
