#include "utils.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>

Fl_Window *window;
Fl_Box* box;
Fl_Input *password;
Fl_Input *input;

void button1Callback(Fl_Widget *widget, void *data) {

    const char* value = input->value();
    const char* pw = password->value();
    encrypt_context(value, pw);
    // Redraw the window
    window->redraw();

}

void button2Callback(Fl_Widget *widget, void *data) {

    const char* value = input->value();
    const char* pw = password->value();
    decrypt_context(value, pw);
    // Redraw the window
    window->redraw();

}


int main() {
    // Create the window
    window = new Fl_Window(300, 250, "FLTK Window");

    box = new Fl_Box(0, 0, window->w(), window->h());
    box->box(FL_NO_BOX);  // Remove the box's border
    box->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);

    // Create the first button
    Fl_Button* button1 = new Fl_Button(100, 160, 100, 40, "Encrypt");
    button1->callback(button1Callback);  // Set the callback function
    button1->parent((Fl_Group*)box);
    Fl_Button* button2 = new Fl_Button(100, 210, 100, 40, "Decrypt");
    button2->callback(button2Callback);  // Set the callback function
    button2->parent((Fl_Group*)box);

    // Adjust the box's size based on the buttons' size
    box->size(button1->w(), button1->h() * 3);

    input = new Fl_Input(20, 40, 260, 30, "");
    input->parent((Fl_Group*)box);
    Fl_Box *label = new Fl_Box(20, 0, 260, 30, "Enter file name:");
    label->parent((Fl_Group*)box);
    password = new Fl_Input(20, 120, 260, 30, "");
    password->parent((Fl_Group*)box);
    Fl_Box *passlabel = new Fl_Box(20, 80, 260, 30, "Enter password:");
    passlabel->parent((Fl_Group*)box);

    // End the creation of the window
    window->end();
    window->show();

    // Run the FLTK event loop
    return Fl::run();
}


