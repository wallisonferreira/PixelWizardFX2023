#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

/*
	STAThread indica que o modelo de threading COM para o aplicativo é um 
	compartimento de thread único. Este atributo deve estar presente no ponto de entrada
	de qualquer aplicação que utilize Windows Forms; se for omitido, os componentes
	do Windows poderão não funcionar corretamente. Se o atributo não estiver presente,
	o aplicativo usará o modelo de compartimento multithread, que não tem suporte para 
	Windows Forms.
*/

[STAThread]
void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	PixelWizardFX2023::MainForm form;

	Application::Run(% form);
}