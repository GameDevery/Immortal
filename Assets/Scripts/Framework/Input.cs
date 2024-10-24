﻿using System.Runtime.CompilerServices;

namespace Immortal
{
    public enum MouseCode : int
    {
        Button0 = 0,
        Button1 = 1,
        Button2 = 2,
        Button3 = 3,
        Button4 = 4,
        Button5 = 5,
        Button6 = 6,
        Button7 = 7,

        Left = Button0,
        Right = Button1,
        Middle = Button2,
        Last = Button7
    };

    public enum KeyCode : int
    {
        Terminator = 0,
        StartOfHeadling,
        StartOfText,
        EndOfText,
        EndOfTransmission,
        Enquiry,
        Acknoledge,
        Bell,
        BackspaceAscii,
        HorizontalTab,
        LineFeed,
        VerticalTab,
        FormFeed,
        CarriageReturn,
        ShiftOut,
        ShiftIn,
        DataLinkEscape,
        DeviceContorl1,
        DeviceContorl2,
        DeviceContorl3,
        DeviceContorl4,
        NegativeAcknoledge,
        SynchronousIdle,
        EndOfTransmissionBlock,
        Cancel,
        EndOfMedium,
        Substitute,
        EscapeAscii,
        FileSeperator,
        GroupSeperator,
        RecordSeperator,
        UnitSeperator,
        Space,
        ExclamationMark,
        DoubleQuotes,
        Number,
        Dollar,
        PercentSign,
        Ampersand,
        SingleQuote,
        OpenParenthesis,
        CloseParenthesis,
        Asterisk,
        Plus,
        Comma,
        Hyphen,
        FullStop,
        Slash,
        D0,
        D1,
        D2,
        D3,
        D4,
        D5,
        D6,
        D7,
        D8,
        D9,
        Colon,
        Semicolon,
        LessThan,
        Equals,
        GreaterThan,
        QuestionMark,
        AtSymbol,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        OpeningBracket,
        BackSlash,
        ClosingBraket,
        CaretCircumflex,
        Underscore,
        GraveAccent,
        a,
        b,
        c,
        d,
        e,
        f,
        g,
        h,
        i,
        j,
        k,
        l,
        m,
        n,
        o,
        p,
        q,
        r,
        s,
        t,
        u,
        v,
        w,
        x,
        y,
        z,
        OpeningBrace,
        VerticalBar,
        ClosingBrace,
        EquivalencySign,

        World1 = 161, /* non-US #1 */
        World2 = 162, /* non-US #2 */

        /* Function keys */
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,

        /* Keypad */
        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,
        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,

        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = LeftShift,
        RightControl = LeftControl,
        RightAlt = LeftAlt,
        RightSuper = 347,
        Menu = 348,

        Shift = LeftShift,
        Control = LeftControl,
        Alt = LeftAlt
    }

    class Input
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public bool GetKeyDown(KeyCode keyCode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public bool GetButtonDown(MouseCode mouseCode);
    }
}
