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
#if DEBUG
        private const string DllName = @"..\..\..\..\..\x64\Debug\Bitmap.dll";
#else
        private const string DllName = @"..\..\..\..\..\x64\Release\Bitmap.dll";
#endif

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateBitmap(string fileName);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DestroyBitmap(IntPtr bitmap);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool CheckIf24Bit(IntPtr bitmap);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetProcessedFragments(IntPtr bitmap, int numOfFragments);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void MergeFragments(IntPtr bitmap, ProcessedFragment[] fragments, int numOfFragments, IntPtr filePath);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool CheckIfProperNumOfThreads(IntPtr bitmap, int numOfThreads);
    }
}
