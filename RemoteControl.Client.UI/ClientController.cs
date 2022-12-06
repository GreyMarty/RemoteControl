using RemoteControl.Net.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using RemoteControl.Client.Core;
using RemoteControl.Encoding;
using RemoteControl.IO.Screen;
using System.Threading;
using RemoteControl.IO.Manipulation;
using System.Diagnostics;
using System.Configuration;

namespace RemoteControl.Client.UI
{
    internal class ClientController
    {
        private Client _client;

        private HostController _hostController;
        private StreamDecoder _decoder;

        private WindowsScreenRecorder _recorder;
        private StreamEncoder _encoder;

        private CommandStreamParser _commandParser;

        private Thread _hostControllerThread;
        private Thread _controlThread;
        private Thread _commandThread;

        private bool _isConnecting = false;

        public string ConnectionString { get; private set; }

        public bool IsConnected => _client?.IsConnected ?? false;
        public bool IsControlling { get; private set; }
        public bool IsControlled { get; private set; }


        public ClientController()
        {
            _client = new Client(IPAddress.Parse(ConfigurationManager.), 40040, 40040);
            _client.TakenControl += TakenControl;
            _client.ForsakenControl += ForsakenControl;
        }

        public async Task ConnectAsync() 
        {
            if (_isConnecting) 
            {
                return;
            }

            var result = NetResult.Error;
            try
            {
                _isConnecting = true;
                result = await _client.ConnectAsync();
            }
            catch (SocketException)
            {
                // ignore
            }
            finally 
            {
                _isConnecting = false;

                if (result != NetResult.Ok)
                {
                    MessageBox.Show(
                        "Could not connect to remote server. Try again later.",
                        "Connection Error",
                        MessageBoxButton.OK,
                        MessageBoxImage.Error
                   );
                }
            }

            ConnectionString = _client.ConnectionString;
        }

        public async Task TakeControlAsync(string connectionString) 
        {
            if (_isConnecting) 
            {
                return;
            }

            var result = NetResult.Error;
            try
            {
                _isConnecting = true;
                result = await _client.TakeControlAsync(connectionString);
            }
            catch (SocketException)
            {
                // ignore
            }
            finally 
            {
                _isConnecting = false;

                if (result != NetResult.Ok)
                {
                    MessageBox.Show(
                        "Could not connect to remote desktop. Check connection string or try again later.",
                        "Connection Error",
                        MessageBoxButton.OK,
                        MessageBoxImage.Error
                    );
                }
            }

            if (result == NetResult.Ok) 
            {
                OpenHostControllerWindow();
                IsControlling = true;
            }
        }

        public async Task ForsakeControlAsync() 
        {
            try
            {
                await _client.ForsakeControlAsync();
            }
            catch (Exception) 
            {
            }

            FreeResources();

            IsControlling = false;
        }

        private void FreeResources() 
        {
            Thread.Sleep(10);

            _hostController?.Close();
            _hostController?.Dispose();

            IsControlled = false;

            _hostControllerThread.Join();
            _decoder?.Dispose();
        }

        private void OpenHostControllerWindow() 
        {
            _decoder = new StreamDecoder(_client.NetworkStream);
            _hostController = new HostController(_decoder, _client.NetworkStream);

            _hostControllerThread = new Thread(_hostController.Show);
            _hostControllerThread.Start();
        }

        private void TakenControl(object sender, string connectionString) 
        {
            _controlThread = new Thread(EncodeScreen);
            _controlThread.Start();

            _commandThread = new Thread(ProcessCommands);
            _commandThread.Start();
        }

        private void ForsakenControl(object sender)
        {
            FreeResources();
        }

        private void EncodeScreen() 
        {
            _recorder = new WindowsScreenRecorder();
            _encoder = new StreamEncoder(_recorder);

            IsControlled = true;

            while (IsControlled) 
            {
                try
                {
                    _encoder.EncodeFrame(_client.NetworkStream);
                }
                catch (Exception e) 
                {
                    Debug.WriteLine($"Encode Screen: {e}");
                }
            }

            _encoder?.Dispose();
            _encoder = null;
            _recorder = null;
        }

        private void ProcessCommands() 
        {
            _commandParser = new CommandStreamParser(_client.NetworkStream);

            IsControlled = true;

            while (IsControlled) 
            {
                try
                {
                    var command = _commandParser.ParseNext();
                    command.Execute();
                    Debug.WriteLine(command);
                }
                catch (Exception e) 
                {
                    Debug.WriteLine($"Process Command: {e}");
                }
            }
        }
    }
}
