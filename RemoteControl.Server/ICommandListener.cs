﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace RemoteControl.Server
{
    public interface ICommandListener : IDisposable
    {
        public int Port { get; }

        public void Run();
    }
}