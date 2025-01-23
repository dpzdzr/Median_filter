using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace JAProjekt
{
    class MedianFilterInterop
    {
#if DEBUG
        private const string DllName = @"..\..\..\..\..\x64\Debug\MedianFilter.dll";
#else
        private const string DllName = @"..\..\..\..\..\x64\Release\MedianFilter.dll";
#endif

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ApplyMedianFilter(IntPtr input, IntPtr output, uint width, uint height);
    }
}
