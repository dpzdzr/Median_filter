using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace JAProjekt
{
    class AsmInterop
    {
        private const string DllName = @"C:\Users\Daniel\OneDrive - Politechnika Śląska\sem5\ja\projekt\JAProjekt\x64\Debug\JAAsm.dll";

        [DllImport(DllName)]
        public static extern int MyProc1(int a, int b);

        [DllImport(DllName)]
        public static extern IntPtr MedianFiltering(IntPtr fragment, IntPtr output);
    }
}
