using Microsoft.Win32;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Imaging;

namespace JAProjekt
{
    public class MainViewModel : INotifyPropertyChanged
    {
        private List<int> _threadOptions;
        private int _selectedThreadCount;

        private List<string> _dllOptions;
        private string _selectedDll;

        private string _selectedFilePath;
        private string _destinationFilePath;

        private BitmapImage _loadedImage;
        private BitmapImage _filteredImage;

        private double _progress;
        private bool _isBusy;

        private string _executionTime;

        private Stopwatch _stopwatch;

        int _computedValue;

        public MainViewModel()
        {
            int threadCount = Environment.ProcessorCount;

            var options = new List<int>();
            for (int i = 1; i <= threadCount; i *= 2)
            {
                options.Add(i);
            }

            ThreadOptions = options;
            SelectedThreadCount = options[0];

            var dlls = new List<string>();
            dlls.Add("C++");
            dlls.Add("ASM");

            DllOptions = dlls;
            SelectedDll = dlls[0];

            MyStopwatch = new Stopwatch();

            BrowseFileCommand = new RelayCommand(_ => OpenFileBrowser());
            //ComputeValue = new RelayCommand(_ => computeValue());
            ProcessBitmapCommand = new RelayCommand(async _ => await ProcessBitmap());
        }

        public ICommand BrowseFileCommand { get; }
        public ICommand ComputeValue { get; }

        public ICommand ProcessBitmapCommand { get; }

        public Stopwatch MyStopwatch
        {
            get => _stopwatch;
            set { _stopwatch = value; OnPropertyChanged(nameof(Stopwatch)); }
        }
        public BitmapImage LoadedImage
        {
            get => _loadedImage;
            set { _loadedImage = value; OnPropertyChanged(nameof(LoadedImage)); }
        }

        public BitmapImage? FilteredImage
        {
            get => _filteredImage;
            set { _filteredImage = value; OnPropertyChanged(nameof(FilteredImage)); }
        }

        public bool IsBusy
        {
            get => _isBusy;
            set { _isBusy = value; OnPropertyChanged(nameof(IsBusy)); }
        }

        public double Progress
        {
            get => _progress;
            set { _progress = value; OnPropertyChanged(nameof(Progress)); }
        }

        public string SelectedFilePath
        {
            get => _selectedFilePath;
            set { _selectedFilePath = value; OnPropertyChanged(nameof(SelectedFilePath)); }
        }

        public string DestinationFilePath
        {
            get => _destinationFilePath;
            set { _destinationFilePath = value; OnPropertyChanged(nameof(DestinationFilePath)); }
        }

        public List<int> ThreadOptions
        {
            get => _threadOptions;
            set { _threadOptions = value; OnPropertyChanged(nameof(ThreadOptions)); }
        }

        public int SelectedThreadCount
        {
            get => _selectedThreadCount;
            set { _selectedThreadCount = value; OnPropertyChanged(nameof(SelectedThreadCount)); }
        }

        public int ComputedValue
        {
            get => _computedValue;
            set { _computedValue = value; OnPropertyChanged(nameof(ComputedValue)); }
        }

        public List<string> DllOptions
        {
            get => _dllOptions;
            set { _dllOptions = value; OnPropertyChanged(nameof(DllOptions)); }
        }

        public string SelectedDll
        {
            get => _selectedDll;
            set { _selectedDll = value; OnPropertyChanged(nameof(SelectedDll)); }
        }

        public string ExecutionTime
        {
            get => _executionTime;
            set { _executionTime = value; OnPropertyChanged(nameof(ExecutionTime)); }
        }

        //public void computeValue()
        //{
        //    _computedValue = AsmInterop.MyProc1(5, 8);
        //    MessageBox.Show(_computedValue.ToString());
        //}

        public void OpenFileBrowser()
        {
            OpenFileDialog openFileDialog = new OpenFileDialog { Filter = "BMP Files|*.bmp" };
            if (openFileDialog.ShowDialog() == true)
            {
                SelectedFilePath = openFileDialog.FileName;

                DestinationFilePath = $"{Path.GetDirectoryName(SelectedFilePath)}\\{Path.GetFileNameWithoutExtension(SelectedFilePath)}MF.bmp";

                LoadedImage = LoadImageToDisplay(SelectedFilePath);
                FilteredImage = null;
            }
        }

        public async Task ProcessBitmap()
        {
            //   Progress = 0;
            IsBusy = true;

            if (string.IsNullOrEmpty(DestinationFilePath))
            {
                MessageBox.Show("Najpierw wybierz plik", "Błąd", MessageBoxButton.OK, MessageBoxImage.Error);
                IsBusy = false;
                return;
            }


            IntPtr bitmapPtr = BitmapInterop.CreateBitmap(SelectedFilePath);

            if (!BitmapInterop.CheckIf24Bit(bitmapPtr))
            {
                MessageBox.Show("Wybrany obraz nie jest w formacie 24-bitowym", "Nieobsługiwany format obrazu",
                    MessageBoxButton.OK, MessageBoxImage.Error);
                IsBusy = false;
                return;
            }

            if (!BitmapInterop.CheckIfProperNumOfThreads(bitmapPtr, SelectedThreadCount))
            {
                MessageBox.Show("Wybrana liczba wątków nie może być większa od wysokości obrazu wyrażonej w pikselach", "Niepoprawna liczba wątków",
                    MessageBoxButton.OK, MessageBoxImage.Error);
                IsBusy = false;
                return;
            }

            try
            {

                IntPtr fragmentsPtr = BitmapInterop.GetProcessedFragments(bitmapPtr, SelectedThreadCount);

                ProcessedFragment[] fragments = new ProcessedFragment[SelectedThreadCount];
                int size = Marshal.SizeOf(typeof(ProcessedFragment));

                for (int i = 0; i < SelectedThreadCount; i++)
                {
                    IntPtr fragmentPtr = new IntPtr(fragmentsPtr.ToInt64() + i * size);
                    fragments[i] = Marshal.PtrToStructure<ProcessedFragment>(fragmentPtr);
                }

                ProcessedFragment[] filteredFragments = null;
                MyStopwatch.Restart();

                if (SelectedDll == "C++")
                {
                    filteredFragments = await ProcessFragmentsAsync(fragments);
                }
                else if (SelectedDll == "ASM")
                {
                    filteredFragments = await ProcessFragmentsAsyncASM(fragments);
                }
                MyStopwatch.Stop();

                IntPtr convDestinationFilePath = Marshal.StringToHGlobalAnsi(DestinationFilePath);
                BitmapInterop.MergeFragments(bitmapPtr, filteredFragments, SelectedThreadCount, convDestinationFilePath);
                Marshal.FreeHGlobal(convDestinationFilePath);

                FilteredImage = LoadImageToDisplay(DestinationFilePath);

                FreeAllFragmentsMemory(filteredFragments);

            }
            finally
            {
                BitmapInterop.DestroyBitmap(bitmapPtr);
                //ResetFilePathInfo();
                IsBusy = false;
                ExecutionTime = MyStopwatch.ElapsedMilliseconds.ToString() + " ms";
            }
        }

        private static async Task<ProcessedFragment[]> ProcessFragmentsAsync(ProcessedFragment[] fragments)
        {
            var tasks = fragments.Select(fragment => Task.Run(() => ApplyMedianFilterToFragment(fragment))).ToArray();

            var filteredFragments = await Task.WhenAll(tasks);

            return filteredFragments;
        }

        private static async Task<ProcessedFragment[]> ProcessFragmentsAsyncASM(ProcessedFragment[] fragments)
        {
            var tasks = fragments.Select(fragment => Task.Run(() => ApplyMedianFilterToFragmentASM(fragment))).ToArray();

            var filteredFragments = await Task.WhenAll(tasks);

            return filteredFragments;
        }

        //private async Task<ProcessedFragment[]> ProcessFragmentsAsync(ProcessedFragment[] fragments)
        //{   
        //    var totalFragments = fragments.Length;
        //    var filteredFragments = new ProcessedFragment[totalFragments];

        //    for (int i = 0; i < totalFragments; i++)
        //    {
        //        filteredFragments[i] = await Task.Run(() => ApplyMedianFilterToFragment(fragments[i]));

        //        Application.Current.Dispatcher.Invoke(() =>
        //        {
        //            Progress = (i + 1) / (double)totalFragments * 100;
        //        });
        //    }
        //    return filteredFragments;
        //}

        public static IntPtr FilterChannel(IntPtr channelData, uint width, uint height)
        {
            int size = (int)(width * height);
            IntPtr outputPtr = Marshal.AllocHGlobal(size);

            MedianFilterInterop.ApplyMedianFilter(channelData, outputPtr, width, height);

            return outputPtr;

        }

        public static IntPtr FilterChannelASM(IntPtr channelData, uint width, uint height)
        {
            int size = (int)((width-2) * (height-2));
            IntPtr outputPtr = Marshal.AllocHGlobal(size);

            AsmInterop.applyFilter(channelData, outputPtr, width, height);

            return outputPtr;

        }

        public static ProcessedFragment ApplyMedianFilterToFragment(ProcessedFragment fragment)
        {
            IntPtr filteredBluePtr = FilterChannel(fragment.Blue, fragment.Width, fragment.Height);
            IntPtr filteredGreenPtr = FilterChannel(fragment.Green, fragment.Width, fragment.Height);
            IntPtr filteredRedPtr = FilterChannel(fragment.Red, fragment.Width, fragment.Height);

            return new ProcessedFragment
            {
                Blue = filteredBluePtr,
                Green = filteredGreenPtr,
                Red = filteredRedPtr,
                StartRow = fragment.StartRow,
                EndRow = fragment.EndRow,
                Width = fragment.Width - 2,
                Height = fragment.Height - 2,
            };
        }

        public static ProcessedFragment ApplyMedianFilterToFragmentASM(ProcessedFragment fragment)
        {
            IntPtr filteredBluePtr = FilterChannelASM(fragment.Blue, fragment.Width, fragment.Height);
            IntPtr filteredGreenPtr = FilterChannelASM(fragment.Green, fragment.Width, fragment.Height);
            IntPtr filteredRedPtr = FilterChannelASM(fragment.Red, fragment.Width, fragment.Height);

            return new ProcessedFragment
            {
                Blue = filteredBluePtr,
                Green = filteredGreenPtr,
                Red = filteredRedPtr,
                StartRow = fragment.StartRow,
                EndRow = fragment.EndRow,
                Width = fragment.Width - 2,
                Height = fragment.Height - 2,
            };
        }

        public static ProcessedFragment[] ApplyMedianFilterToFragments(ProcessedFragment[] fragments)
        {
            var filteredFragments = new ProcessedFragment[fragments.Length];
            for (int i = 0; i < filteredFragments.Length; i++)
            {
                var filteredFragment = ApplyMedianFilterToFragment(fragments[i]);
                filteredFragments[i] = filteredFragment;
            }
            return filteredFragments;
        }

        public static void FreeFragmentMemory(ProcessedFragment fragment)
        {
            Marshal.FreeHGlobal(fragment.Blue);
            Marshal.FreeHGlobal(fragment.Green);
            Marshal.FreeHGlobal(fragment.Red);
        }

        public static void FreeAllFragmentsMemory(ProcessedFragment[] fragments)
        {
            foreach (var fragment in fragments)
            {
                FreeFragmentMemory(fragment);
            }
        }

        public static BitmapImage LoadImageToDisplay(string filePath)
        {
            BitmapImage image = new BitmapImage();
            image.BeginInit();
            image.CacheOption = BitmapCacheOption.OnLoad;
            image.CreateOptions = BitmapCreateOptions.IgnoreImageCache;
            image.UriSource = new Uri(filePath);
            image.EndInit();
            image.Freeze();

            return image;
        }
        public void ResetFilePathInfo()
        {
            SelectedFilePath = string.Empty;
            DestinationFilePath = string.Empty;
        }

        public event PropertyChangedEventHandler? PropertyChanged;
        protected void OnPropertyChanged(string propertyName) =>
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}