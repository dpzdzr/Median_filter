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
        private const string DllName = @"C:\Users\Daniel\Desktop\ja\japrojekt\x64\Debug\MedianFilter.dll";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ApplyMedianFilter(IntPtr input, IntPtr output, uint width, uint height);
    }
}
