[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/StQAS8iq)

My continous cellular automata is inspired by grayscale automata concepts.

I use a 3x3 average of neighbor cell values adding a constant to the average, and taking the fractional part of result
to assign a new value to each cell. The difference between my greyscale CA, is that I only apply the update,
as designed by Wolfram Alpha, to cells with initial values above 0. 

This design, creates an intial wave-like pattern followed by dynamic pods of life appearing and fading
out throughout the grid. Intensity and life is visualezed with HSV.

Sources I used for my implementation are:
Stephen Wolfram's A New Kind of Science | Online
- https://www.wolframscience.com/nks/p158--continuous-cellular-automata/

Stephen Silver's Life Lexicon
- https://conwaylife.com/ref/lexicon/lex_home.htm