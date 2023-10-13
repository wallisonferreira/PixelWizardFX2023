#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

/*
	STAThread indica que o modelo de threading COM para o aplicativo � um 
	compartimento de thread �nico. Este atributo deve estar presente no ponto de entrada
	de qualquer aplica��o que utilize Windows Forms; se for omitido, os componentes
	do Windows poder�o n�o funcionar corretamente. Se o atributo n�o estiver presente,
	o aplicativo usar� o modelo de compartimento multithread, que n�o tem suporte para 
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