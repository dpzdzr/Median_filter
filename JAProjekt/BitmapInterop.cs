using System.Runtime.InteropServices;

namespace JAProjekt
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ProcessedFragment
    {
        public IntPtr Blue;   
        public IntPtr Green;   
        public IntPtr Red;     
        public uint StartRow;
        public uint EndRow;
        public uint Width;
        public uint Height;
    }

    class BitmapInterop
    {
        private const string DllName = @"C:\Users\Daniel\Desktop\ja\japrojekt\x64\Release\Bitmap.dll";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateBitmap(string fileName);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DestroyBitmap(IntPtr bitmap);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern Boolean CheckIf24Bit(IntPtr bitmap);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetProcessedFragments(IntPtr bitmap, int numOfFragments);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void MergeFragments(IntPtr bitmap, ProcessedFragment[] fragments, int numOfFragments, IntPtr filePath);
    }
}
