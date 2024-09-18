import pandas as pd
import json
import re

# Supongamos que has guardado tu JSON en un archivo llamado 'resultados.json'
file_path = r'C:\Users\dwaisman\Desktop\scrapeo_MAIN\scrapeo_granos\aceites\resultados.json'

# Leer el archivo JSON
with open(file_path, 'r', encoding='utf-8') as f:
    data = json.load(f)

# Convertir la lista de diccionarios a un DataFrame
df = pd.DataFrame(data)

# Función para extraer mes y año de la URL
def extraer_mes_y_año(url):
    match = re.search(r'/(\w{3})_(\d{2})/', url)
    if match:
        mes, año = match.groups()
        return mes, '20' + año
    return None, None

# Aplicar la función a la columna 'url'
df['mes'], df['año'] = zip(*df['url'].apply(extraer_mes_y_año))

# Función para limpiar y truncar 'dato_especifico'
def limpiar_y_truncar_dato(dato):
    if pd.isna(dato):
        return None
    # Eliminar puntos de miles
    dato = dato.replace('.', '')
    dato = dato.replace(',', '')

    return dato

# Aplicar la función de limpieza y truncado
df['dato_especifico'] = df['dato_especifico'].apply(limpiar_y_truncar_dato)

# Convertir la columna 'dato_especifico' a numérico
df['dato_especifico'] = pd.to_numeric(df['dato_especifico'], errors='coerce')

# Describir los datos
descripcion = df.describe()

# Guardar el DataFrame y la descripción en un archivo Excel
excel_file_path = r'C:\Users\dwaisman\Desktop\scrapeo_MAIN\scrapeo_magyp/resultados_descriptivos_granos_finales.xlsx'
with pd.ExcelWriter(excel_file_path) as writer:
    df.to_excel(writer, sheet_name='Datos', index=False)
    descripcion.to_excel(writer, sheet_name='Descripcion')

print(f"Datos y descripción guardados en {excel_file_path}")
