#pragma once

namespace Project1 {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::IO::Ports;
    using namespace System::Windows::Forms::DataVisualization::Charting;

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();
        }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::ComponentModel::Container^ components;
        System::Windows::Forms::Label^ label1;
        SerialPort^ serialPort;
        Chart^ chart1;  // Grafik kontrolü
        array<double>^ ivmeData;
        array<double>^ basincData;
        array<double>^ sicaklikData;
        int dataIndex = 0;  // Veri indeksini takip eder
        const int dataSize = 50;  // Veri dizilerinin boyutunu artırdık

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->Location = System::Drawing::Point(375, 18);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(915, 247);
            this->label1->TabIndex = 1;
            this->label1->Text = L"Veri burada görünecek";
            this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
            // 
            // chart1
            // 
            this->chart1->Location = System::Drawing::Point(320, 282);
            this->chart1->Name = L"chart1";
            this->chart1->Size = System::Drawing::Size(1068, 438);
            this->chart1->TabIndex = 2;
            this->chart1->Text = L"chart1";
            this->chart1->Click += gcnew System::EventHandler(this, &MyForm::chart1_Click);
            // 
            // MyForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1815, 766);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->chart1);
            this->Name = L"MyForm";
            this->Text = L"MyForm";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
            this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
            this->ResumeLayout(false);

        }
#pragma endregion

        // Form yüklenirken seri portu başlat
        void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
            serialPort = gcnew SerialPort("COM3", 9600);  // COM3 olarak güncellendi
            serialPort->DataReceived += gcnew SerialDataReceivedEventHandler(this, &MyForm::SerialPort_DataReceived);

            // Grafik verilerini başlat
            ivmeData = gcnew array<double>(dataSize);
            basincData = gcnew array<double>(dataSize);
            sicaklikData = gcnew array<double>(dataSize);

            try
            {
                serialPort->Open();  // Seri portu aç
                label1->Text = L"Seri port açıldı.";
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Seri port açılamadı: " + ex->Message);
            }
        }

        // Seri port üzerinden veri alındığında tetiklenen olay
        void SerialPort_DataReceived(System::Object^ sender, SerialDataReceivedEventArgs^ e)
        {
            String^ data = serialPort->ReadLine();  // Seri porttan gelen veriyi oku
            this->Invoke(gcnew Action<String^>(this, &MyForm::UpdateData), data);  
        }

        // Verileri güncelle ve grafikte göster
        void UpdateData(String^ data)
        {
            array<String^>^ values = data->Split(',');

            if (values->Length == 3)
            {
                try
                {
                    // Veriyi uygun aralıklara dönüştür
                    // ivme değeri 500 ile 1500 arasında alınıp 100.0 ile bölünür
                    double ivme = (random(500, 1500)) / 100.0; // 5.00 ile 15.00 arasında ivme

                    // Basınç değeri 900 ile 1100 arasında olmalı
                    int basinc = random(900, 1100);

                    // Sicaklik -200 ile 500 arasında alınıp 10.0 ile bölünür
                    float sicaklik = (random(-200, 500)) / 10.0;  // -20.0 ile 50.0 arasında sıcaklık

                    // Veri dizilerine ekle
                    ivmeData[dataIndex] = ivme;
                    basincData[dataIndex] = basinc;
                    sicaklikData[dataIndex] = sicaklik;

                    // Veri indeksini arttır
                    dataIndex = (dataIndex + 1) % dataSize;

                    // Label güncelle
                    label1->Text = "İvme: " + ivme + "\nBasınç: " + basinc + "\nSıcaklık: " + sicaklik;

                    // Grafikteki verileri güncelle
                    UpdateChart();
                }
                catch (FormatException^)
                {
                    // Hata mesajı göster, giriş formatı hatalıysa
                    MessageBox::Show("Veri formatı hatalı!");
                }
            }
        }

        // Grafiği güncelle
        void UpdateChart()
        {
            // Grafik alanını ve serileri oluştur
            if (chart1->ChartAreas->Count == 0)
            {
                // İlk defa ChartArea ekleniyor
                ChartArea^ chartArea = gcnew ChartArea();
                chart1->ChartAreas->Add(chartArea);
            }

            // Grafik serilerini oluştur
            chart1->Series->Clear();

            // Serileri oluştur
            Series^ ivmeSeries = gcnew Series("İvme");
            ivmeSeries->Points->DataBindY(ivmeData);
            ivmeSeries->ChartType = SeriesChartType::Line; // Çizgi grafiği
            ivmeSeries->BorderWidth = 3;
            ivmeSeries->Color = Color::Blue; // Mavi renk

            Series^ basincSeries = gcnew Series("Basınç");
            basincSeries->Points->DataBindY(basincData);
            basincSeries->ChartType = SeriesChartType::Line; // Çizgi grafiği
            basincSeries->BorderWidth = 3;
            basincSeries->Color = Color::Green; // Yeşil renk

            Series^ sicaklikSeries = gcnew Series("Sıcaklık");
            sicaklikSeries->Points->DataBindY(sicaklikData);
            sicaklikSeries->ChartType = SeriesChartType::Line; // Çizgi grafiği
            sicaklikSeries->BorderWidth = 3;
            sicaklikSeries->Color = Color::Red; // Kırmızı renk

            // Grafikteki serileri ekle
            chart1->Series->Add(ivmeSeries);
            chart1->Series->Add(basincSeries);
            chart1->Series->Add(sicaklikSeries);
        }

        // Form kapandığında seri portu kapat
        void MyForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
            if (serialPort->IsOpen)
            {
                serialPort->Close();  // Seri portu kapat
            }
        }

        // Random aralık içinde değer döndüren fonksiyon
        int random(int minValue, int maxValue)
        {
            Random^ rand = gcnew Random();
            return rand->Next(minValue, maxValue + 1);
        }

    private: System::Void chart1_Click(System::Object^ sender, System::EventArgs^ e) {
    }
    private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
    }
    };
}