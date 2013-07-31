Apsis is a general purpose game engine written in C++ (with some assembly bits and GLSL shaders). Currently, it has an OpenGL/SDL backend.

## System Design

It is designed to be able to build games using objects, actors, and rules. For instance, if you create an engine that has a Map, a player Actor and attach Up, Down, Left, Right, and MapCollision rules, you will produce a 2d top-down scroller, ala Legend of Zelda. If you remove the Up and Down rules from the player Actor and replace them with Fall and Jump rules, you will now have a 2d side-scroller platformer!

![Screenshot](http://dave-wilkinson.herokuapp.com/images/cv/apsis.png)

## Motivation

The grand scheme of things is to produce something that can be used by both the experienced game designer to quickly produce new games, and to allow the same ease of use to teach beginners (including kids, but also curious adults!) how to code. The idea is that most of the game logic can be done with the given rules and tools (yet to be written). It will use the old learning technique of "Learn when you want to do something special" and have learners write their game by introducing new rules (small, modular) that they can code themselves. The hope is to allow such rules to be written in a variety of languages that have better support or culture for beginners (ruby, python, lua, javascript, etc).

## Initial Focus

The focus is on desktop games for Windows, OS X, and Linux using some modern hardware. Objects and Actors in the system will provide both 2d and 3d components. Contributors should help out in providing new rules, discussion of design, and art! LOTS OF ART in the creative commons, please. I also hope to have auto-attribution of art for licenses that require it.

## Team

The team is just me, wilkie. I am a systems researcher and implementor that has an interest in education (I have taught, assisted, and mentored several courses directed at beginners.) I also run the [Open Source Game Coding Competition](http://osgcc.github.io) geared toward pairing beginners with experts to create games in a social space entering its seventh year. I am very knowledgeable in systems level implementation and yet still comfortable with front-end design. I shall openly admit my collaborators using javascript insist that I do not touch the stuff, though!

## Contributing

If this project is of interest to you, please help! Ping me (wilkie) in freenode IRC or create an issue entitled "I need something to do!!" and I'll get back to you. Things you can do:

* I've been doing a lot of the development on Windows (gaming machine has the most juice, and no working Linux install) so... getting a build script for OS X and Linux.
* 64-bit support for atomic operations.
* Adding support for model animation.
* Adding support for interesting material types for 3d purposes.
* Considering mobile support.
* Discuss design! It's still early on.
* Contributing to creative commons art collections (like openart)
* Create lots of issues for feature requests and documentation.
