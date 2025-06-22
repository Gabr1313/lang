#import io

main : int : {
    x := 1                         // can change value, infer the type `int`
    x++;                           // `;` is optional
    y :: 2                         // is constant
    z : f32 : 2                    // specify the type too
    x = pow! x, y-x                // `!` is a function call
    w := pow? z, (pow! x, y)       // `?` is a function call with error propagation
                                   // forced to use `()` in nested function calls
    w, err := pow? z, (pow! x, y)  // in case you want to handle the error
    if err  return 1               // any value != null
    x := "Ciao mamma"              // shadowing of a variable is legal
    io.println! "x thought: %", x  // `%`, infers it is a string, and knows how to print it

    w #= pow! 3, 5                 // comp time (the compiler should do it anyway, but this way it is forced to)

    return 0
}

pow : int : int x, int y { // `i32` != `int` since the latter is the C `size_t`
    z := x
    if y < 0  return 0
    if y == 0 return x
    for _ in 0..y {
        z *= x
    }
    return z
}

pow : f32, err : f32 x, int y { // overloading a function
    if y < 0   return 0, "Not implemented yet"
    if y == 0  return x
    z : f32 = x
    for in 0..y  z *= x         // inline for syntax, without any `{` nor `(`
    return z, null
}
