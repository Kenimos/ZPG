# README
VIDEO: https://youtu.be/8dXu7QorD1Q

## Přepínání scén za běhu
- **Implementováno:** Ano
- **Kde a jak:** 
  - Třída `App` v metodě `processInput()`
  - Klávesy 1-9 pro přepínání na jednotlivé scény

## Scény
### a) Základní scéna
- **Implementováno:** Ano
- **Popis:** Scéna 3 vykresluje jednoduchý trojúhelník pomocí Phongova shaderu

### b) Scéna s lesem
- **Implementováno:** Ano
- **Popis:** Scéna 1 načítá modely stromů a keřů, generuje 50 stromů a 50 keřů s náhodnými pozicemi, rotacemi a měřítky

### c) Scéna se čtyřmi kuličkami
- **Implementováno:** Ano
- **Popis:** Scéna 2 obsahuje čtyři kuličky, každá s jiným shaderem (Konstantní, Lambert, Phong, Blinn-Phong) pro porovnání vizuálních rozdílů

### d) Scéna pro demonstraci použití všech shaderů
- **Implementováno:** Ano
- **Popis:** Součást scény 2 umožňuje přepínání shaderů stisknutím klávesy X, mění shader u všech objektů ve scéně

## Světlo
- **Implementováno:** Ano
- **Kde a jak:**
  - Třída `Light`
  - Inicializace v metodě `initShaders()` třídy `Scene`
  - Použití Observer pattern pro aktualizaci shaderů při změně vlastností světla
- **Aktualizace:** V metodě `draw()` scény notifikace shaderů o aktuálním stavu světla

## Základní třídy
- **ShaderProgram:**
  - Správa shaderových programů, kompilace shaderů, nastavování uniform proměnných
- **DrawableObject:**
  - Reprezentace vykreslitelných objektů, obsahuje model, shader, transformace
- **Camera:**
  - Správa pozice kamery, výpočet view a projection matrix
  - Subjekt pozorovaný shadery pro aktualizaci
- **Controller:**
  - Implementováno v třídě `App`, zpracovává uživatelské vstupy a řídí hlavní smyčku aplikace
- **Vykreslování:**
  - Třída `Scene` v metodě `draw()`, iterace přes `DrawableObject` a volání jejich `draw()`
- **Modely a Shadery:**
  - Modely uloženy v třídě `Model`, načítány ve `Scene` metodách `loadModelsSceneN()`
  - Shadery načítány a kompilovány ve `ShaderProgram` pomocí `ShaderLoader`, inicializace v `initShaders()`

## Transformace (Composite pattern)
- **Implementováno:** Ano
- **Popis:** Třída `Transformation` kombinuje pozici, rotaci a škálování, poskytuje metodu `getModelMatrix()`
- **Poznámka:** Nebyl použit explicitní Composite pattern, ale kombinace základních transformací je efektivní

## Základy OOP
### a) Encapsulation (Zapouzdření)
- **Implementováno:** Ano
- **Popis:** Privátní datové členy s veřejnými metodami pro přístup a modifikaci
- **Příklad:** `DrawableObject` s privátními členy `model`, `shaderProgram`, `transformation` a veřejnými metodami

### b) Inheritance (Dědičnost)
- **Implementováno:** Ano
- **Popis:** Využití v Observer patternu
  - `Camera` a `Light` dědí od `Subject`
  - `ShaderProgram` dědí od `Observer`

### c) Polymorphism (Polymorfismus)
- **Implementováno:** Ano
- **Popis:** V rámci Observer patternu
  - Virtuální metoda `update()` v základní třídě `Observer`, přepsaná v `ShaderProgram`
