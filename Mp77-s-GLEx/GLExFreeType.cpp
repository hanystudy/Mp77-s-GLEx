#include "GLExFreeType.h"

freetype::font_data our_font;

GLExFreeType::GLExFreeType()
{
	our_font.init("arial.TTF", 12);					    //Build the freetype font
	memset(displaydevice,0,sizeof(displaydevice));
	memset(displaydeviceid,0,sizeof(displaydeviceid));
}


GLExFreeType::~GLExFreeType(void)
{

}

void GLExFreeType::PrintSystemInfo()
{
	GLfloat cur[4];
	glGetFloatv(GL_CURRENT_COLOR,cur);
	glColor4f(0.5,0.5,0.0,1.0);
	glPushMatrix();
	glLoadIdentity();
	systeminfo->GetDisplayDeviceString(displaydevice);
	systeminfo->GetDisplayDeviceID(displaydeviceid);
	freetype::print(our_font, 0, height - 12, "Mp77's GLExtent based on NEHE tutorials\n"
											"Display Device: %s\n"
											"Display Device Id: %s\n"
											"Frame rate: %.1f fps\nCPU Usage: %.1f%%\n"
											"Total Working Set Size: %.0fK\n"
											"Peak Working Set Size: %.0fK\n"
											"Page File Usage: %.0fK\n"
											"Peak Page File Useage: %.0fK\n",displaydevice,displaydeviceid,systeminfo->GetFrameRate(),systeminfo->GetCpuUsage(),systeminfo->GetWorkingSetSize(),systeminfo->GetPeakWorkingSetSize(),systeminfo->GetPagefileUsage(),systeminfo->GetPeakPagefileUsage());
	glPopMatrix();
}

void GLExFreeType::SetSystemInfo(GLExSystemInfo *si)	
{
	systeminfo = si;
}

void GLExFreeType::SetWindowSize(GLfloat w ,GLfloat h)
{
	this->width = w;
	this->height = h;
}

void GLExFreeType::PrintSystemOption(bool tex)
{
	GLfloat cur[4];
	glGetFloatv(GL_CURRENT_COLOR,cur);
	glColor4f(0.5,0.5,0.0,1.0);
	glPushMatrix();
	glLoadIdentity();
	QString itex;
	if(tex)
		itex = "On";
	else
		itex = "Off";
	freetype::print(our_font, width - 300, height - 12, "Textures: %s(Press 'T/t' to On/Off Textures)\nFiltering Mode: Mipmapping\n",itex.toLocal8Bit().data());
	glPopMatrix();
}