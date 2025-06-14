#pragma once

#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"

namespace Buttons
{

enum Button
{
    Unknown = (int)sf::Keyboard::Key::Unknown,
    A = (int)sf::Keyboard::Key::A,
    B = (int)sf::Keyboard::Key::B,
    C = (int)sf::Keyboard::Key::C,
    D = (int)sf::Keyboard::Key::D,
    E = (int)sf::Keyboard::Key::E,
    F = (int)sf::Keyboard::Key::F,
    G = (int)sf::Keyboard::Key::G,
    H = (int)sf::Keyboard::Key::H,
    I = (int)sf::Keyboard::Key::I,
    J = (int)sf::Keyboard::Key::J,
    K = (int)sf::Keyboard::Key::K,
    L = (int)sf::Keyboard::Key::L,
    M = (int)sf::Keyboard::Key::M,
    N = (int)sf::Keyboard::Key::N,
    O = (int)sf::Keyboard::Key::O,
    P = (int)sf::Keyboard::Key::P,
    Q = (int)sf::Keyboard::Key::Q,
    R = (int)sf::Keyboard::Key::R,
    S = (int)sf::Keyboard::Key::S,
    T = (int)sf::Keyboard::Key::T,
    U = (int)sf::Keyboard::Key::U,
    V = (int)sf::Keyboard::Key::V,
    W = (int)sf::Keyboard::Key::W,
    X = (int)sf::Keyboard::Key::X,
    Y = (int)sf::Keyboard::Key::Y,
    Z = (int)sf::Keyboard::Key::Z,
    Num0 = (int)sf::Keyboard::Key::Num0,
    Num1 = (int)sf::Keyboard::Key::Num1,
    Num2 = (int)sf::Keyboard::Key::Num2,
    Num3 = (int)sf::Keyboard::Key::Num3,
    Num4 = (int)sf::Keyboard::Key::Num4,
    Num5 = (int)sf::Keyboard::Key::Num5,
    Num6 = (int)sf::Keyboard::Key::Num6,
    Num7 = (int)sf::Keyboard::Key::Num7,
    Num8 = (int)sf::Keyboard::Key::Num8,
    Num9 = (int)sf::Keyboard::Key::Num9,
    Escape = (int)sf::Keyboard::Key::Escape,
    LControl = (int)sf::Keyboard::Key::LControl,
    LShift = (int)sf::Keyboard::Key::LShift,
    LAlt = (int)sf::Keyboard::Key::LAlt,
    LSystem = (int)sf::Keyboard::Key::LSystem,
    RControl = (int)sf::Keyboard::Key::RControl,
    RShift = (int)sf::Keyboard::Key::RShift,
    RAlt = (int)sf::Keyboard::Key::RAlt,
    RSystem = (int)sf::Keyboard::Key::RSystem,
    Menu = (int)sf::Keyboard::Key::Menu,
    LBracket = (int)sf::Keyboard::Key::LBracket,
    RBracket = (int)sf::Keyboard::Key::RBracket,
    Semicolon = (int)sf::Keyboard::Key::Semicolon,
    Comma = (int)sf::Keyboard::Key::Comma,
    Period = (int)sf::Keyboard::Key::Period,
    Apostrophe = (int)sf::Keyboard::Key::Apostrophe,
    Slash = (int)sf::Keyboard::Key::Slash,
    Backslash = (int)sf::Keyboard::Key::Backslash,
    Grave = (int)sf::Keyboard::Key::Grave,
    Equal = (int)sf::Keyboard::Key::Equal,
    Hyphen = (int)sf::Keyboard::Key::Hyphen,
    Space = (int)sf::Keyboard::Key::Space,
    Enter = (int)sf::Keyboard::Key::Enter,
    Backspace = (int)sf::Keyboard::Key::Backspace,
    Tab = (int)sf::Keyboard::Key::Tab,
    PageUp = (int)sf::Keyboard::Key::PageUp,
    PageDown = (int)sf::Keyboard::Key::PageDown,
    End = (int)sf::Keyboard::Key::End,
    Home = (int)sf::Keyboard::Key::Home,
    Insert = (int)sf::Keyboard::Key::Insert,
    Delete = (int)sf::Keyboard::Key::Delete,
    Add = (int)sf::Keyboard::Key::Add,
    Subtract = (int)sf::Keyboard::Key::Subtract,
    Multiply = (int)sf::Keyboard::Key::Multiply,
    Divide = (int)sf::Keyboard::Key::Divide,
    Left = (int)sf::Keyboard::Key::Left,
    Right = (int)sf::Keyboard::Key::Right,
    Up = (int)sf::Keyboard::Key::Up,
    Down = (int)sf::Keyboard::Key::Down,
    Numpad0 = (int)sf::Keyboard::Key::Numpad0,
    Numpad1 = (int)sf::Keyboard::Key::Numpad1,
    Numpad2 = (int)sf::Keyboard::Key::Numpad2,
    Numpad3 = (int)sf::Keyboard::Key::Numpad3,
    Numpad4 = (int)sf::Keyboard::Key::Numpad4,
    Numpad5 = (int)sf::Keyboard::Key::Numpad5,
    Numpad6 = (int)sf::Keyboard::Key::Numpad6,
    Numpad7 = (int)sf::Keyboard::Key::Numpad7,
    Numpad8 = (int)sf::Keyboard::Key::Numpad8,
    Numpad9 = (int)sf::Keyboard::Key::Numpad9,
    F1 = (int)sf::Keyboard::Key::F1,
    F2 = (int)sf::Keyboard::Key::F2,
    F3 = (int)sf::Keyboard::Key::F3,
    F4 = (int)sf::Keyboard::Key::F4,
    F5 = (int)sf::Keyboard::Key::F5,
    F6 = (int)sf::Keyboard::Key::F6,
    F7 = (int)sf::Keyboard::Key::F7,
    F8 = (int)sf::Keyboard::Key::F8,
    F9 = (int)sf::Keyboard::Key::F9,
    F10 = (int)sf::Keyboard::Key::F10,
    F11 = (int)sf::Keyboard::Key::F11,
    F12 = (int)sf::Keyboard::Key::F12,
    F13 = (int)sf::Keyboard::Key::F13,
    F14 = (int)sf::Keyboard::Key::F14,
    F15 = (int)sf::Keyboard::Key::F15,
    Pause = (int)sf::Keyboard::Key::Pause,
    _keyboardEnd,

    /* -----Mouse----- */

    MouseLeft,
    MouseRight,
    MouseMiddle,
    MouseExtra1,
    MouseExtra2,
};

constexpr Button toButton(sf::Keyboard::Key key)
{
    return static_cast<Button>(key);
}

constexpr Button toButton(sf::Mouse::Button button)
{
    return static_cast<Button>(static_cast<int>(button) + _keyboardEnd + 1);
}

constexpr bool isKeyboard(Button button) { return button < _keyboardEnd; }

constexpr bool isMouse(Button button) { return button > _keyboardEnd; }

}
