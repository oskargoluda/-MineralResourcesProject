# Wizualizacja Geologiczna 3D

Ten projekt to aplikacja do wizualizacji 3D stworzona przy użyciu OpenGL, GLFW i GLM do wyświetlania modeli geologicznych różnych złóż mineralnych. Aplikacja umożliwia użytkownikom przeglądanie i interakcję z modelami 3D regionów górniczych, w tym Legnica-Głogów (miedź), Kłodawa (sól), Mełgiew (gaz ziemny) oraz Adamów (węgiel brunatny). Oferuje sterowanie kamerą, płaszczyzny przycinania dla widoków przekrojowych oraz billboardy z opisowymi teksturami.

## Funkcjonalności

- **Renderowanie modeli 3D**:  
  Wyświetlanie modeli 3D formacji geologicznych wczytywanych z plików `.obj` przy użyciu biblioteki `tinyobjloader`.

- **Interaktywna kamera**:  
  Obrót, zbliżanie i przesuwanie kamery wokół wybranego modelu za pomocą klawiatury i myszy.

- **Płaszczyzny przycinania**:  
  Przełączanie między trybami przycinania góra-dół i lewo-prawo, umożliwiającymi przeglądanie przekrojów modeli.

- **Billboardy**:  
  Wyświetlanie teksturowanych billboardów nad każdym modelem z informacjami opisowymi, zwróconych w stronę kamery.

- **Wsparcie dla tekstur**:  
  Nakładanie tekstur na modele i billboardy dla lepszej szczegółowości wizualnej.

- **Oświetlenie**:  
  Implementacja podstawowego oświetlenia z wieloma światłami kierunkowymi i efektami ambientowymi.

## Zależności

- **OpenGL**: Do renderowania grafiki 3D  
- **GLEW**: Do obsługi rozszerzeń OpenGL  
- **GLFW**: Do zarządzania oknem i wejściem  
- **GLM**: Do operacji na macierzach i wektorach  
- **tinyobjloader**: Do wczytywania plików modeli `.obj`  
- **Własny loader shaderów**: Do wczytywania i zarządzania shaderami GLSL  
- **Loader tekstur**: Do wczytywania tekstur w formacie PNG

## Konfiguracja i instalacja

Sklonuj repozytorium:

git clone https://github.com/oskargoluda/MineralResourcesProject.git

Uruchom w Visual Studio 2022 

**Sterowanie:**

- **W / S** – Zmiana elewacji kamery (góra/dół)  
- **A / D** – Obrót kamery (azymut)  
- **Q / E** – Przybliżanie/oddalanie kamery  
- **Kółko myszy** – Zmiana odległości kamery  
- **1** – Legnica-Głogów  
- **2** – Kłodawa  
- **3** – Mełgiew  
- **4** – Adamów  
- **C** – Przełączanie między trybami (OFF, TOP-DOWN, LEFT-RIGHT)  
- **R / F** – Przesuwanie płaszczyzny przycinania w trybie aktywnym  
- **ESC** – Wyjście z programu
