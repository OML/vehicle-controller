using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;


using System.Runtime.InteropServices;


enum Tab2CarSteerMode
{
        XY,
        Throttle
};

[StructLayout(LayoutKind.Sequential, Pack=1)]
struct Tab2CarSteer
{
        byte x;
        byte y;
};


[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct Tab2CarMotor
{
        UInt16 Throttle; // Promille
        UInt16 Voltage; // Millivolts
        UInt16 Current; // Milliamperes
        UInt16 Temperature; // Millidegrees
};

[StructLayout(LayoutKind.Sequential, Pack=1)]
struct Tab2CarPacket
{
        public Byte Command;
        public Byte SteerMode;
        public Tab2CarSteer Steer;
        public Tab2CarMotor M0;
        public Tab2CarMotor M1;
        public Tab2CarMotor M2;
        public Tab2CarMotor M3;
}

namespace prot_tab2car
{
        static class Program
        {
                /// <summary>
                /// The main entry point for the application.
                /// </summary>
                [STAThread]
                static void Main()
                {
                        unsafe {
                                byte[] buffer = new byte[sizeof(Tab2CarPacket)];
                                Tab2CarPacket* pack = null;

                                // Vertel de Garbage Collector dat hij zijn tengels van onze buffer af moet houden:
                                GCHandle gch = GCHandle.Alloc(buffer);
                                pack = (Tab2CarPacket*)gch.AddrOfPinnedObject().ToPointer();

                                // Doe je ding met de variabelen
                                pack->SteerMode = (byte)Tab2CarSteerMode.Throttle;

                                // En uiteindelijk:
                                gch.Free();

                                // Alles in de buffer is nu gewijzigd naar wat jij wil.
                        }
                }
        }
}
