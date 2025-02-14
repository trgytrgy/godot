/**************************************************************************/
/*  gl_manager_windows.h                                                  */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef GL_MANAGER_WINDOWS_H
#define GL_MANAGER_WINDOWS_H

#if defined(WINDOWS_ENABLED) && defined(GLES3_ENABLED)

#include "core/error/error_list.h"
#include "core/os/os.h"
#include "core/templates/local_vector.h"
#include "servers/display_server.h"

#include <windows.h>

typedef bool(APIENTRY *PFNWGLSWAPINTERVALEXTPROC)(int interval);
typedef int(APIENTRY *PFNWGLGETSWAPINTERVALEXTPROC)(void);

class GLManager_Windows {
public:
	enum ContextType {
		GLES_3_0_COMPATIBLE,
	};

private:
	// any data specific to the window
	struct GLWindow {
		int width = 0;
		int height = 0;
		bool use_vsync = false;

		// windows specific
		HDC hDC;
		HWND hwnd;

		int gldisplay_id = 0;
	};

	struct GLDisplay {
		// windows specific
		HGLRC hRC;
	};

	RBMap<DisplayServer::WindowID, GLWindow> _windows;
	LocalVector<GLDisplay> _displays;

	GLWindow *_current_window = nullptr;

	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

	// funcs
	void _internal_set_current_window(GLWindow *p_win);

	GLWindow &get_window(unsigned int id) { return _windows[id]; }
	const GLWindow &get_window(unsigned int id) const { return _windows[id]; }

	const GLDisplay &get_current_display() const { return _displays[_current_window->gldisplay_id]; }
	const GLDisplay &get_display(unsigned int id) { return _displays[id]; }

	bool direct_render;
	int glx_minor, glx_major;
	ContextType context_type;

private:
	int _find_or_create_display(GLWindow &win);
	Error _create_context(GLWindow &win, GLDisplay &gl_display);

public:
	Error window_create(DisplayServer::WindowID p_window_id, HWND p_hwnd, HINSTANCE p_hinstance, int p_width, int p_height);
	void window_destroy(DisplayServer::WindowID p_window_id);
	void window_resize(DisplayServer::WindowID p_window_id, int p_width, int p_height);

	// get directly from the cached GLWindow
	int window_get_width(DisplayServer::WindowID p_window_id = 0);
	int window_get_height(DisplayServer::WindowID p_window_id = 0);

	void release_current();
	void make_current();
	void swap_buffers();

	void window_make_current(DisplayServer::WindowID p_window_id);

	Error initialize();

	void set_use_vsync(DisplayServer::WindowID p_window_id, bool p_use);
	bool is_using_vsync(DisplayServer::WindowID p_window_id) const;

	HDC get_hdc(DisplayServer::WindowID p_window_id);
	HGLRC get_hglrc(DisplayServer::WindowID p_window_id);

	GLManager_Windows(ContextType p_context_type);
	~GLManager_Windows();
};

#endif // WINDOWS_ENABLED && GLES3_ENABLED

#endif // GL_MANAGER_WINDOWS_H
