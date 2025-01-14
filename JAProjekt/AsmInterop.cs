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
        private const string DllName = @"C:\Users\Daniel\Desktop\ja\japrojekt\x64\Release\JAAsm.dll";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void applyFilter(IntPtr fragment, IntPtr output, uint width, uint height);
    }
}
