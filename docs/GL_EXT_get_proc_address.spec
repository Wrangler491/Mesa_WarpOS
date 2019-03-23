Name

    EXT_get_proc_address

Name Strings

    GL_EXT_get_proc_address

Contact

    Brian Paul, (brian_paul 'at' mesa3d.org)

Status

    Planned for Mesa 3.1 and other OpenGL implementations.

Version

    $Id: GL_EXT_get_proc_address.spec,v 1.5 1999/10/13 19:10:50 brianp Exp $

Number

    ???

Dependencies

    The extension is written against the OpenGL 1.2 Specification.

Overview

    This extension adds a function (GetProcAddressEXT) which returns
    the address of extension functions, given the function name.  Since
    the availability of extensions (and their functions) cannot be
    determined until runtime, the address of extension functions
    should be queried at runtime.  This function provides a simple,
    portable way to do so.

Issues

  * Is this extension properly named?

        Yes.  EXT_get_func_address would be another option though.

  * Should a typedef be used for the return type?

        typedef void (*GLfunction)();
        extern GLfunction glGetProcAddressEXT( const GLubyte *procName );

        Not needed.  The following works:

        void (*glGetProcAddressEXT(const GLubyte *procName))();

  * Should GetProcAddressEXT allow querying of itself?

        Yes, for sake of completeness.

  * There's a recursion problem with this feature.  The purpose of
    GetProcAddressEXT is to return pointers to extension functions
    and GetProcAddressEXT is itself such a function!  This presents
    a puzzle to the application developer.

        A Linux/OpenGL Standard Base-compliant OpenGL library will
        always have GetProcAddressEXT.

  * Should extension functions in GLU, GLX, WGL, etc. be queryable through
    this extension?

        No, the companion functions gluGetProcAddressEXT,
        glXGetProcAddressEXT, etc. should be used instead.
        Otherwise, all sorts of module abstraction violations have
        to be dealt with.

  * Should the core functions added in OpenGL 1.1 and 1.2 also be queryable?

        Yes.  This will allow maximum portability of applications across
        OpenGL 1.0, 1.1 and 1.2 library implementations.

  * Are function pointers context-dependent?

        No.  The pointer to an extension function can be used with any
        context which supports the extension.

        There was concern that function dispatch for context-independent
        pointers wouldn't be as efficient as context-dependent pointers.
        After discussion it was agreed that the performance difference
        would be negligible or nonexistant.

New Procedures and Functions

    void (*GetProcAddressEXT(const GLubyte *procName))()

New Tokens

    None.

Additions to Chapter 2 of the OpenGL 1.2 Specification (OpenGL Operation)

    None

Additions to Chapter 3 of the OpenGL 1.2.1 Specification (Rasterization)

    None

Additions to Chapter 4 of the OpenGL 1.2.1 Specification (Per-Fragment
Operations and the Frame Buffer)

    None

Additions to Chapter 5 of the OpenGL 1.2.1 Specification (Special 
Functions)

    None

Additions to Chapter 6 of the OpenGL 1.2 Specification (State and State
Requests)

    Add a new section numbered 6.1.13:

    6.1.13 Obtaining Extension Function Pointers

    The GL extensions which are available to a client application can
    only be determined at runtime.  Therefore, the address of extension
    functions should be queried at runtime.

    The function
        void (*)() GetProcAddressEXT(const ubyte *procName);
    returns the address of the extension function named by procName.

    The pointer returned by GetProcAddressEXT should be cast to a
    function type which matches the extension function's definition.

    If GetProcAddressEXT returns a non-NULL pointer it is not guaranteed
    that the particular extension is available.  The client application
    must also query GetString(EXTENSIONS) to determine if the extension
    is present.

    The address returned by GetProcAddressEXT is independent of the
    currently bound context and may be used by any context which supports
    the extension.  Furthermore, if the function entrypoint is also
    exported by the library, the following is true:

        GetProcAddressEXT(<function>) = &<function>

    For example:
        GetProcAddressEXT("glActiveTextureARB") = &glActiveTextureARB

    GetProcAddressEXT can only be used to query functions inside the
    main GL library.  Specifically, extension functions in GLU, GLX, WGL,
    etc. are not obtainable with this function.

Additions to Appendix A of the OpenGL 1.2.1 Specification (Invariance)

    None

Additions to the GLX / WGL / AGL Specifications

    None

GLX Protocol

    ???

Errors

    As with other client-side queries, calling GetProcAddressEXT between
    Begin and End is not allowed but may or may not generate an
    INVALID_OPERATION error.  If an error is not generated, GL operation
    is undefined.

New State

    None

Revision History

  * Revision 1.1 - initial draft (8 Sep 1999)

  * Revision 1.2 - (9 Sep 1999)
                 - don't allow querying of GLU, GLX, WGL functions
                 - changed return type to void (*)()
                 - allow querying of all core functions except those
                   defined since version 1.0
                 - clarification of GetProcAddressEXT behavior with
                   respect to GL contexts
                 - updated the Issues section

  * Revision 1.3 - (16 Sep 1999)
                 - clarification of the return type issue

  * Revision 1.4 - (13 Oct 1999)
                 - function pointers are now context independent and
                   equivalent to the address of the function if it's
                   exported by the GL library.

  * Revision 1.5 - (13 Oct 1999)
                 - calling GetProcAddressEXT between Begin/End may or may
                   not generate an error.
