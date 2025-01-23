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
#if DEBUG
        private const string DllName = @"..\..\..\..\..\x64\Debug\JAAsm.dll";
#else
        private const string DllName = @"..\..\..\..\..\x64\Release\JAAsm.dll";
#endif

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void applyFilter(IntPtr fragment, IntPtr output, uint width, uint height);
    }
}
