# Diseño 3D — Encapsulado

> Los datos de esta página vienen de una conversación anterior sobre la caja en Fusion 360, no están verificados con los archivos de diseño. Todo lo marcado como `COMPLETAR` falta.

## Archivos exigidos

| Entregable | Archivo | Estado |
| :--- | :--- | :--- |
| Modelo editable de Fusion 360 | `diseno-3d/modelo/sumidero.f3d` (o `.f3z`) | COMPLETAR |
| Render exterior | `diseno-3d/renders/exterior.png` | COMPLETAR |
| Render interior (con ESP32, sensor y powerbank) | `diseno-3d/renders/interior.png` | COMPLETAR |
| Planos técnicos (PDF) | `diseno-3d/planos/planos.pdf` | COMPLETAR |

El **gemelo 3D** debe incluir el encapsulado y los componentes internos. Los planos deben mostrar vistas de varios lados, medidas, escala y tolerancias.

## Descripción del diseño

| Aspecto | Dato |
| :--- | :--- |
| Software | Fusion 360 |
| Material y fabricación | PLA, impresión 3D |
| Montaje | Colgada bajo una rejilla de sumidero SERVIU S2, con el sensor mirando hacia abajo, hacia el agua |
| Sellado | Caja completamente cerrada, con todo adentro. Método de sellado: COMPLETAR (empaquetadura, silicona, etc.) |
| Fijación a la rejilla | Tornillos en los costados, no en el fondo, para no comprometer el sellado |
| Tapa | Tornillos M3 con insertos de bronce termofijados de 3 × 5 × 4 mm (rosca × diámetro exterior × largo). La cabeza del tornillo puede sobresalir (sin avellanado) |
| Paso del cable del sensor | Debe pasar un conector tipo USB; la abertura de una versión anterior era demasiado pequeña y se debía agrandar |

## Medidas del contenido

| Elemento | Medidas | Estado |
| :--- | :--- | :--- |
| Conjunto ESP32 en PCB perforada | 60 × 40 mm, 38 mm de alto | Medido, final |
| Powerbank | 100 × 70 × 25 mm | Final |
| Powerbank, opciones evaluadas antes | 100 × 65 × 50 mm y 100 × 70 × 20 mm | Descartadas |

## Datos que faltan

| Dato | Valor |
| :--- | :--- |
| Dimensiones exteriores de la caja | COMPLETAR |
| Espesor de paredes | COMPLETAR |
| Tolerancias | COMPLETAR |
| Piezas (cuerpo, tapa, soportes...) y secuencia de ensamblaje | COMPLETAR |
| Parámetros de impresión (altura de capa, relleno, temperatura, orientación) | COMPLETAR |
| ¿La sonda del sensor queda dentro o fuera de la caja? Distancia desde la sonda al nivel del agua | COMPLETAR |
