Esercizio sull'utilizzo di ROOT e C++ standard insieme simulando un esperimento di fisica delle particelle.
La parte del codice scritta solo in C++ è stata compilata con g++ nei seguenti modi:
- g++ ParticleType.cpp ResonanceType.cpp Particle.cpp test1.cpp (per testare il codice con l'utilizzo di doctest, necessita doctest.h e C++ 11)
- g++ ParticleType.cpp ResonanceType.cpp Particle.cpp test2.cpp (per testare le funzioni di stampa)\
\
Il resto del codice è stato compilato all'interno di root, nei seguenti modi:
- .L ParticleType.cpp+
- .L ResonanceType.cpp+
- .L Particle.cpp+\
per creare delle shared library (.so) utilizzate nel main.\
\
Fatto ciò si prosegue con:
- .L main.cpp
- main()
- .x Analysis.C
