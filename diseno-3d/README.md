# Diseño 3D — Encapsulado

> Diseño del encapsulado del sistema detector de nivel anormal en sumideros, modelado en Fusion 360. Los datos de esta página corresponden a los planos técnicos del 22-11-2026.

## Archivos entregados

Los archivos se encuentran en la carpeta [`diseno-3d`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/tree/main/diseno-3d) del repositorio del proyecto.

| Entregable | Archivo |
| :--- | :--- |
| Modelo editable de Fusion 360 | [`modelo/sumidero.f3d`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/diseno-3d/modelo/sumidero.f3d) |
| Render exterior | [`renders/exterior.png`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/diseno-3d/renders/exterior.png) |
| Render interior (con ESP32, sensor y powerbank) | [`renders/interior.png`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/diseno-3d/renders/interior.png) |
| Render de montaje bajo la rejilla | [`renders/montaje_rejilla.png`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/diseno-3d/renders/montaje_rejilla.png) |
| Planos técnicos (PDF, 3 láminas A3) | [`planos/planos.pdf`](https://github.com/JoaquinZavalaM/Monitor-Sumidero/blob/main/diseno-3d/planos/planos.pdf) |

El **gemelo 3D** incluye el encapsulado, los componentes internos (powerbank, conjunto ESP32 y módulo del sensor) y la rejilla del sumidero. Los planos muestran vista frontal, lateral derecha y superior de cada pieza, con medidas, escala y tolerancias.

## Renders

### Exterior

![Render exterior del encapsulado](https://raw.githubusercontent.com/JoaquinZavalaM/Monitor-Sumidero/main/diseno-3d/renders/exterior.png)

Caja cerrada vista desde abajo: la sonda del sensor sale por el collarín de la tapa, apuntando hacia el agua. Arriba se ven las pestañas con las tuercas M6 y las barras de apriete.

### Interior

![Render interior con los componentes](https://raw.githubusercontent.com/JoaquinZavalaM/Monitor-Sumidero/main/diseno-3d/renders/interior.png)

Tapa con los componentes montados: el powerbank (negro) apoyado sobre la tapa y, encima, el conjunto ESP32 en PCB perforada (gris con placa verde) junto al módulo del sensor (azul). Se ve la ranura de paso del cable hacia la sonda.

### Montaje bajo la rejilla

![Render del montaje bajo la rejilla](https://raw.githubusercontent.com/JoaquinZavalaM/Monitor-Sumidero/main/diseno-3d/renders/montaje_rejilla.png)

Encapsulado colgado bajo una rejilla de sumidero, con el sensor mirando hacia abajo.

## Descripción del diseño

| Aspecto | Dato |
| :--- | :--- |
| Software | Fusion 360 |
| Material y fabricación | PLA, impresión 3D |
| Montaje | Colgada bajo una rejilla de sumidero SERVIU S2, con el sensor mirando hacia abajo, hacia el agua |
| Sellado | Caja completamente cerrada, con todo adentro. Cordón de goma Ø2 mm en una ranura de junta de 2,6 × 1,5 mm en el borde de apoyo de la tapa. La ranura de paso del cable se sella con silicona o epoxi. |
| Fijación a la rejilla | 4 pestañas en el techo de la caja se apoyan bajo la rejilla. 4 pernos M6 (de 25 a 30 mm, con golilla ancha) bajan desde 2 barras de apriete apoyadas sobre la rejilla, pasan entre las barras de la rejilla y se atornillan a tuercas M6 alojadas bajo las pestañas. No hay perforaciones en el fondo ni en las paredes, para no comprometer el sellado. |
| Tapa | Cierra la caja por abajo con 4 tornillos M3 × 8 o M3 × 10 (sin avellanado) en insertos de bronce termofijados de 3 × 5 × 4 mm (rosca × diámetro exterior × largo), ubicados en las orejas de la caja. |
| Sonda del sensor | Queda **fuera** de la caja: va encajada en un collarín de la tapa (Ø22 interior / Ø30 exterior, 21 mm de alto) y sobresale 2 mm, con la cara emisora hacia el agua. |
| Paso del cable del sensor | Ranura de 14 × 7 mm en la tapa, dentro del collarín, dimensionada para el conector USB-A. Se sella con silicona o epoxi. |

## Piezas

| Pieza | Cantidad | Medidas principales | Lámina |
| :--- | :--- | :--- | :--- |
| Caja (cuerpo del encapsulado) | 1 | 140 × 88 × 95 mm; con orejas y pestañas 140 × 126 × 95 mm | 1/3 |
| Tapa (fondo, con collarín de la sonda) | 1 | 140 × 112 × 4 mm; altura total 25 mm con el collarín | 2/3 |
| Barra superior (placa de apriete) | 2 | 140 × 30 × 8 mm, 2 agujeros Ø6,5 a 100 mm entre ejes | 3/3 |

**Elementos comerciales:**

| Elemento | Cantidad |
| :--- | :--- |
| Inserto de bronce termofijado M3 (3 × 5 × 4 mm) | 4 |
| Tornillo M3 × 8 o M3 × 10 | 4 |
| Perno M6 de 25 a 30 mm con golilla ancha | 4 |
| Tuerca M6 | 4 |
| Cordón de goma Ø2 mm (junta de la tapa) | 1 |
| Silicona o epoxi (sellado del paso del cable) | — |

## Medidas de la caja

| Dato | Valor |
| :--- | :--- |
| Dimensiones exteriores del cuerpo | 140 × 88 × 95 mm |
| Dimensiones con orejas y pestañas | 140 × 126 × 95 mm |
| Cavidad interior | 130 × 78 × 91 mm, abierta hacia abajo (la cierra la tapa) |
| Espesor de paredes | 5 mm |
| Espesor del techo | 4 mm |
| Espesor de la tapa | 4 mm |
| Radio de esquinas | 3 mm |
| Pestañas de apoyo | 4 × (18 × 19 × 8 mm), a ras del techo |
| Agujeros de los pernos M6 | Ø6,5 mm pasante, ejes a 100 × 108 mm |
| Alojamientos de tuerca M6 | Hexagonales de 10,4 mm entre caras × 5 mm de profundidad |
| Agujeros para insertos M3 | Ø4,6 × 6 mm ciegos, ejes a 128 × 100 mm |
| Agujeros de la tapa | 4 × Ø3,4 mm pasante, ejes a 128 × 100 mm |
| Collarín de la sonda | Ø22 interior / Ø30 exterior, 21 mm de alto, eje a 52 mm del centro de la tapa |

## Medidas del contenido

| Elemento | Medidas | Estado |
| :--- | :--- | :--- |
| Conjunto ESP32 en PCB perforada | 60 × 40 mm, 38 mm de alto | Medido, final |
| Powerbank | 100 × 70 × 25 mm | Final |
| Sonda del sensor | Ø22 × 17 mm | Final |
| Powerbank, opciones evaluadas antes | 100 × 65 × 50 mm y 100 × 70 × 20 mm | Descartadas |

El powerbank (100 × 70 mm) cabe en la cavidad interior (130 × 78 mm). Sobre él se ubican el conjunto ESP32 y el módulo del sensor, dentro de los 91 mm de altura de la cavidad.

## Tolerancias

| Aspecto | Valor |
| :--- | :--- |
| Tolerancia general | ±0,2 mm, salvo indicación |
| Agujero para inserto M3 | Ø4,6 mm, a calibrar con una pieza de prueba de 4,5 / 4,6 / 4,7 mm |
| Agujero de la sonda | Ø22 mm; agregar 0,2 mm si la sonda real mide exactamente 22 mm |
| Agujeros de pernos M6 | Ø6,5 mm (0,5 mm de holgura) |
| Agujeros de tornillos M3 en la tapa | Ø3,4 mm (0,4 mm de holgura) |

## Secuencia de ensamblaje

1. Imprimir la caja, la tapa y las 2 barras superiores en PLA.
2. Termofijar los 4 insertos M3 en los agujeros de las orejas de la caja.
3. Colocar las 4 tuercas M6 en los alojamientos hexagonales bajo las pestañas.
4. Instalar el cordón de goma Ø2 mm en la ranura de junta de la caja.
5. Encajar la sonda en el collarín de la tapa, pasar el cable por la ranura de 14 × 7 mm y sellarla con silicona o epoxi.
6. Ubicar el powerbank, el conjunto ESP32 y el módulo del sensor sobre la tapa y conectar el cable de la sonda.
7. Cerrar la caja con la tapa y los 4 tornillos M3.
8. Ubicar la caja bajo la rejilla, con las pestañas apoyadas contra ella, y las 2 barras superiores sobre la rejilla.
9. Pasar los 4 pernos M6 con golilla ancha desde las barras, entre las barras de la rejilla, y apretarlos en las tuercas de la caja.

