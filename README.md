The Game of M Solver, using N numbers
===========================

It started as a [Game of 24](https://en.wikipedia.org/wiki/24_Game)  (also known as the 24 Game)solver. 
The program was enhanced: Now it can solve any variants on this game, reaching any arbitrary number by combining at most 17 numbers if memory permits.

The program outputs a list of all possible solutions to the given goal M and the number set with length n. An option to stop after finding just one solution is also created.

As the complexity of this problem quickly rises, as can be seen when analysing the number of possibilities:

``P(n) = C(n-1) * n! * 4^(n-1) ``
(Where C(n) is the Catalan Number function, and n! is the factorial of n)

<table>
<tr>
  <td>
    n
  </td>
  <td>
    P(n) = C(n-1) * n! * 4^(n-1)
  </td>
</tr>
<tr>
  <td>
    1
  </td>
  <td>
    1
  </td>
</tr>
<tr>
  <td>
    2
  </td>
  <td>
    8
  </td>
</tr>
<tr>
  <td>
    3
  </td>
  <td>
    192
  </td>
</tr>

<tr>
  <td>
    4
  </td>
  <td>
    7680
  </td>
</tr>

<tr>
  <td>
    5
  </td>
  <td>
    430080
  </td>
</tr>

<tr>
  <td>
    6
  </td>
  <td>
    30965760
  </td>
</tr>

<tr>
  <td>
    7
  </td>
  <td>
    2724986880
  </td>
</tr>

<tr>
  <td>
    8
  </td>
  <td>
    283398635520
  </td>
</tr>

<tr>
  <td>
    9
  </td>
  <td>
    34007836262400
  </td>
</tr>

<tr>
  <td>
    10
  </td>
  <td>
    4625065731686400
  </td>
</tr>

<tr>
  <td>
    11
  </td>
  <td>
    703009991216332800
  </td>
</tr>

<tr>
  <td>
    12
  </td>
  <td>
    118105678524343910400
  </td>
</tr>

<tr>
  <td>
    13
  </td>
  <td>
    21731444848479279513600
  </td>
</tr>

<tr>
  <td>
    14
  </td>
  <td>
    4346288969695855902720000
  </td>
</tr>

<tr>
  <td>
    15
  </td>
  <td>
    938798417454304874987520000
  </td>
</tr>

<tr>
  <td>
    16
  </td>
  <td>
    217801232849398730997104640000
  </td>
</tr>

<tr>
  <td>
    17
  </td>
  <td>
    54014705746650885287281950720000
  </td>
</tr>

<tr>
  <td>
    18
  </td>
  <td>
    14259882317115833715842434990080000
  </td>
</tr>
</table>
(See [Integer Sequence A052734](http://oeis.org/A052734) )


This means that the program, while working with larger collections of numbers, will run for a very long time. 
Also, although the program tries to limit memory use, still a significant amount of memory needs to be reserved, namely C(n-1).
(See [Integer Sequence A000108](http://oeis.org/A000108) )


However, it should be very possible to paralellize this algorithm and run it on a supercomputer.

About
=====

This code was created for Introduction to Computing Science at the University of Groningen in 2014.

More information about this project can be found in the report:
[http://wmcode.nl/cs/gameofmnReport.pdf](http://wmcode.nl/cs/gameofmnReport.pdf)

