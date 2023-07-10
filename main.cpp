#include "utils.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>

Fl_Window *window;
Fl_Secret_Input *password;
Fl_Input *input;
Fl_Input *seed;

void popup(const char* text) {
    Fl_Window *popup = new Fl_Window(200, 100, "Error");
    popup->set_modal();

    Fl_Box* box = new Fl_Box(-50, -100, window->w(), window->h(), text);
    box->box(FL_NO_BOX);
    box->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);

    popup->end();
    popup->show();

    Fl::run();
}

void encrypt_button_function(Fl_Widget *widget, void *data) {
    //Get the data and pass into the encrypting function
    const char* value = input->value();
    if (!*value) {
        popup("Please provide a filename!");
        return;
    }
    const char* pw = password->value();
    if (!*pw) {
        popup("Please provide a password!");
        return;
    }
    const char* seedc = seed->value();
    if (!*seedc) {
        popup("Please provide a seed!");
        return;
    }
    int err = encrypt_context(value, pw, seedc);
    switch(err) {
        case 1:
            popup("Memory allocation failure!");
            break;
        case 2:
            popup("File does not exist!");
            break;
        case 3:
            popup("Oeps, something weird happened. Maybe you're lacking file editing permissions?");
            break;
        case 4:
            popup("File corrupted!");
            break;
        default:
            popup("Operation succeded!");
    }
    // Redraw the window
    window->redraw();

}

void decrypt_button_function(Fl_Widget *widget, void *data) {
    //Get the data and pass into the encrypting function
    const char* value = input->value();
    if (!*value) {
        popup("Please provide a filename!");
        return;
    }
    const char* pw = password->value();
    if (!*pw) {
        popup("Please provide a password!");
        return;
    }
    const char* seedc = seed->value();
    if (!*seedc) {
        popup("Please provide a seed!");
        return;
    }
    int err = decrypt_context(value, pw, seedc);
    switch(err) {
        case 1:
            popup("Memory allocation failure!");
            break;
        case 2:
            popup("File does not exist!");
            break;
        case 3:
            popup("Oeps, something weird happened. Maybe you're lacking file editing permissions?");
            break;
        case 4:
            popup("File corrupted!");
            break;
        default:
            popup("Operation succeded!");
    }
    // Redraw the window
    window->redraw();

}


int main() {
    // Create the window
    window = new Fl_Window(300, 330, "File encrypter");

    // Box to store our elements in
    Fl_Box* box = new Fl_Box(0, 0, window->w(), window->h());
    box->box(FL_NO_BOX);
    box->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);

    // Creating buttons and assigning functions to them
    Fl_Button* encryptButton = new Fl_Button(100, 240, 100, 40, "Encrypt");
    encryptButton->callback(encrypt_button_function);
    encryptButton->parent((Fl_Group*)box);
    Fl_Button* decryptButton = new Fl_Button(100, 290, 100, 40, "Decrypt");
    decryptButton->callback(decrypt_button_function);
    decryptButton->parent((Fl_Group*)box);

    // Adjust the box's size based on the buttons' size
    box->size(encryptButton->w(), encryptButton->h() * 3);

    // Initializing buttons and inputs
    input = new Fl_Input(20, 40, 260, 30, "");
    input->parent((Fl_Group*)box);//Filename
    Fl_Box *label = new Fl_Box(20, 0, 260, 30, "Enter file name:");
    label->parent((Fl_Group*)box);
    password = new Fl_Secret_Input(20, 120, 260, 30, "");
    password->parent((Fl_Group*)box);//Password
    Fl_Box *passlabel = new Fl_Box(20, 80, 260, 30, "Enter password:");
    passlabel->parent((Fl_Group*)box);
    seed = new Fl_Input(20, 200, 260, 30, "");
    seed->parent((Fl_Group*)box);//Seed to randomly generate salt
    Fl_Box *seedlabel = new Fl_Box(20, 160, 260, 30, "Enter seed:");
    seedlabel->parent((Fl_Group*)box);
    // End the creation of the window
    window->end();
    window->show();

    // Run the FLTK event loop
    return Fl::run();
}


