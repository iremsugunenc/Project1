#include "MyForm.h"  // Form dosyasını dahil ediyoruz

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]  
int Main()
{
   
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    
    Project1::MyForm form;

    
    Application::Run(% form);

    return 0;
}
