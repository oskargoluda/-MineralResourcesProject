# Teksturowanie
Te zajęcia są kontynuacją poprzednich Zalecam wykorzystać ukończony kod z poprzednich zajęć. Projekt startowy jest tylko częściowo ukończonymi zadaniami

### Zadanie
Obejrzyj plik **Texture.h**, aby zapoznać się z interfejsem do obsługi tekstur.


### Przygotuj shadery i załaduj teksturę:
W kolejnych zadaniach będziemy potrzebować trzecią parę shaderów: **shader_5_tex.vert** i **shader_5_tex.frag** są one gotowymi shaderami z poprzednich zadań.  Wczytaj shadery i przechowaj adres programu pod zmienną globalną `programTex`.

Pod `namespace texture` znajdują się zmienne globalne typu `Gluint`, które będą przechowywać adresy tekstur.  W funkcji `init()` załaduj tekstury z folderu **textures** przy pomocy `Core::LoadTexture`. Ta funkcja zwraca adres do wczytanej tekstury, przypisz je do zmiennych z  `namespace texture`.  (odwołanie się do `namespace` wykonuje się za pomocą ::, czyli by odwołać się do `grid` w `namespace` `texture` należy napisać `texture::grid`)

Stwórz funkcje odpowiadająca za rysowanie teksturowanych obiektów. Skopiuj funkcję `drawObjectColor`. Nazwij kopię `drawObjectTexture()` i zmodyfikują ją tak, aby przyjmowała jako parametr identyfikator tekstury, a nie wektor koloru. (Usuń w niej linijkę odpowiadającą za przesyłanie koloru, żeby uniknąć błędu komunikacji, później dodamy na jej miejsce ładowanie tekstury). 



### Zmodyfikuj shader tak, aby nakładał teksturę na obiekt:
Narysuj jedną z planet za pomocą `drawObjectTexture()`, użyj `programTex` jako programu.

b) Prześlij współrzędne mapowania tekstur z *vertex shadera* do *fragment shadera*
- Współrzędne tekstur to kolejny (po pozycjach i wektorach normalnych) atrybut wierzchołków - są dostępne w *vertex shaderze* pod nazwą `vertexTexCoord`
- Prześlij je znanym już sposobem do *fragment shadera* (zmienna `out` w *vertex shaderze* i odpowiadająca jej zmienna `in` we *fragment shaderze*)

c) Prześlij teksturę do fragment shadera:
- Stwórz zmienną typu `uniform sampler2D` (nazwij ją na przykład `colorTexture`) we fragment shaderze - analogicznie do innych zmiennych typu uniform, służy ona do przesyłania informacji bezpośrednio z kodu C++ do shadera
- Po stronie kodu C++ użyj funkcji `Core::SetActiveTexture` wewnątrz `drawObjectTexture()`  aby ustawić zmienną `sampler2D` na wczytaną wcześniej teksturę.

d) Użyj wartości uzyskanej z tekstury zamiast koloru (`objectColor`) do pokolorowania obiektu:
- Wykonaj próbkowanie tekstury we współrzędnych otrzymanych przez fragment shader:  `vec4 textureColor = texture2D(~nazwaZmiennejSampler2D, ~nazwaZmiennejWspolrzedneTekstury)`" (`vec4` zawiera kolor RGBA)
- Użyj pierwszych trzech współrzędnych (RGB) uzyskanego wektora jako nowego koloru bazowego piksela.


### Pobaw się mechanizmem teksturowania:
- Przemnóż jedną lub obie ze współrzędnych mapowania przez 5 i sprawdź, co się stanie.
- Wypróbuj pozostałe tekstury: **grid_color.png**, **earth.png**.
- Tekstury Ziemi wyświetlają się "do góry nogami". Napraw to.
- Jeśli chcesz mieć kilka planet o różnych teksturach, możesz skorzystać z [link](https://www.solarsystemscope.com/textures/) lub [link2](https://stevealbers.net/albers/sos/sos.html).

### Zadanie * 
Nadaj jakieś tekstury wszystkim obiektom, które znajdują się w scenie. 

## Multitexturing 
W tej części pokażemy jak wykorzystać więcej niż jedną teksturę przy rysowaniu obiektu na przykładzie zachmurzonej Ziemi. 

### Zadanie
- Utwórz kolejną parę shaderów, nazwij je `shader_earth`, skopiuj poprzednie shadery, jakie wykorzystywaliśmy, 
- dodaj w nich nowy `uniform sampler2D` nazwij go `clouds`,
- użyj do rysowania ziemi nowo stworzonego shadera,
- wyślij dodatkowo teksturę chmur, która znajduje się pod plikiem `clouds.png`, ustaw indeks tekstury na **2**,
* by sprawdzić, czy całość się udała, użyj chmury jako kolory.
 
Wykorzystamy teksturę chmur jako maskę. Ustaw kolor Ziemi jako mix między białym a kolorem tekstury Ziemi a za współczynnik mieszania weź kanał `r` tekstury chmur.

### Zadanie*
W podobny sposób wykorzystaj tekstury `ship.jpg`, `scratches.jpg` i `rust.jpg`, żeby dodać zarysowania do statku. Załaduj wszystkie trzy tekstury, użyj `scratches.jpg` jako maskę, która będzie wskazywała współczynnik, którym będziemy mieszać pomiędzy dwoma pozostałymi teksturami. 

## Teksturowanie proceduralne

a) Stwórz czwartą parę plików z shaderami (np. **shader_proc_tex.vert** i **shader_proc_tex.frag**). Następnie zainicjalizuj program jak w I.6.a) i II.1.a) (nazwij go np `programProcTex`). Do wywołania rysowania wykorzystuj funkcję `drawObject` (Żeby nie tracić oteksturowanych planet, możesz rysować za ich pomocą wyłącznie statek).

b) Prostym sposobem proceduralnego teksturowania, jest uzależnienie koloru od pozycji piksela w przestrzeni lokalnej (użycie przestrzeni świata spowodowałoby, że wzór na obiekcie zmieniałby się przy poruszaniu go).
- Prześlij z vertex shadera do fragment shadera pozycję wierzchołka w przestrzeni lokalnej (czyli tej, w której wyrażone są atrybuty wierzchołka - nie trzeba więc wykonywać żadnej transformacji macierzowej).
- We fragment shaderze oblicz sinus współrzędnej y pozycji piksela.
- Jeżeli sinus jest większy od zera, to ustaw bazowy kolor obiektu na wybrany kolor, a jeśli jest mniejszy od zera, to na inny kolor.
- Możesz przesłać te kolory przy użyciu zmiennych uniform z kodu C++ - pozwoli to rysować różne obiekty z różnymi parami kolorów.
- Poeksperymentuj z innymi metodami teksturowania proceduralnego podanymi na wykładzie.

