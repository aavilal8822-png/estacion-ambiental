# -*- coding: utf-8 -*-
"""
Gemelo digital 3D — Estación Ambiental IoT TEI201

EJECUTAR DENTRO DE AUTODESK FUSION (Scripts and Add-Ins), no en Python normal.
Crea un NUEVO diseño, importa las mallas STL originales en milímetros y
construye 5 componentes internos a volumen referencial usando BRep.
Intenta exportar en el mismo directorio: Gemelo_3D_Estacion_Ambiental.f3d.

PLAN B: Si alguna operación de modelado no funciona en la versión de Fusion,
abre otro diseño nuevo e importa el ensamble .3mf validado previamente.

Los cuerpos de sensores y ESP32 son volúmenes de referencia; NO se ha
certificado que encajen, ni se han verificado tolerancias, cableado o montajes.
"""
import os
import traceback
import adsk.core
import adsk.fusion

CARPETA = os.path.dirname(os.path.abspath(__file__))
CAJA = os.path.join(CARPETA, 'caja_estacion_ambiental.stl')
TAPA = os.path.join(CARPETA, 'tapa_estacion_ambiental.stl')
ENSAMBLE_3MF = os.path.join(CARPETA, 'Gemelo_3D_Estacion_Ambiental.3mf')
SALIDA_F3D = os.path.join(CARPETA, 'Gemelo_3D_Estacion_Ambiental.f3d')

# Cotas extraídas de la malla original de la caja: vértice mínimo (mm).
ORIGEN = (355.994385, -41.941994, -45.527855)
# Tapa desplazada para que se vea separada encima del cuerpo.
MOV_TAPA = (296.63, 0, 55.0)
# x/y centros relativos a la esquina mínima de la caja. Z desde el fondo.
# Ancho X / largo Y / alto Z, todo expresado en MILÍMETROS.
PIEZAS = [
    ('03 Batería portátil (135x65x20 mm)',    48,   122,   4, (65, 135, 20)),
    ('04 ESP32-S3 (volumen referencial)',   122,   119,   5, (30, 68, 13)),
    ('05 DSM501A (derecha superior)',      195.5, 167,   5, (58, 36, 23)),
    ('06 MQ-9 (derecha cuadrado)',         194,   102,   5, (36, 32, 4)),
    ('07 DHT11 (frente circular)',         209,    25,  11, (20, 12, 12)),
]


def en_cm(v):
    return float(v) / 10.0


def p3(x, y, z):
    return adsk.core.Point3D.create(en_cm(x), en_cm(y), en_cm(z))


def v3(x, y, z):
    return adsk.core.Vector3D.create(en_cm(x), en_cm(y), en_cm(z))


def crear_diseno(app):
    app.documents.add(adsk.core.DocumentTypes.FusionDesignDocumentType)
    dis = adsk.fusion.Design.cast(app.activeProduct)
    if dis is None:
        raise RuntimeError('El documento nuevo no es un diseño de Fusion.')
    # DirectDesignType permite crear sólidos y mallas sin BaseFeature.
    dis.designType = adsk.fusion.DesignTypes.DirectDesignType
    return dis, dis.rootComponent


def nuevo_componente(raiz, nombre, traslacion=(0, 0, 0)):
    m = adsk.core.Matrix3D.create()
    m.translation = v3(*traslacion)
    occ = raiz.occurrences.addNewComponent(m)
    if not occ:
        raise RuntimeError('No se pudo crear el componente ' + nombre)
    c = occ.component
    c.name = nombre
    return c


def cargar_stl(raiz, archivo, nombre, traslacion=(0, 0, 0)):
    c = nuevo_componente(raiz, nombre, traslacion)
    cuerpos = c.meshBodies.add(archivo, adsk.fusion.MeshUnits.MillimeterMeshUnit)
    if cuerpos is None or cuerpos.count == 0:
        raise RuntimeError('Fusion no ha importado: ' + archivo)
    for i in range(cuerpos.count):
        cuerpos.item(i).name = nombre
    return cuerpos


def caja_solida(comp, nombre, xc, yc, zc, tam_x, tam_y, tam_z):
    obb = adsk.core.OrientedBoundingBox3D.create(
        p3(xc, yc, zc),
        adsk.core.Vector3D.create(1, 0, 0),
        adsk.core.Vector3D.create(0, 1, 0),
        en_cm(tam_x), en_cm(tam_y), en_cm(tam_z)
    )
    temp = adsk.fusion.TemporaryBRepManager.get().createBox(obb)
    if temp is None:
        raise RuntimeError('No se pudo crear volumen ' + nombre)
    cuerpo = comp.bRepBodies.add(temp)
    if cuerpo is None:
        raise RuntimeError('No se pudo guardar volumen ' + nombre)
    cuerpo.name = nombre


def cilindro_solido(comp, nombre, xc, yc, z_de, z_hasta, r):
    temp = adsk.fusion.TemporaryBRepManager.get().createCylinderOrCone(
        p3(xc, yc, z_de), en_cm(r), p3(xc, yc, z_hasta), en_cm(r)
    )
    if temp is None:
        raise RuntimeError('No se pudo crear cilindro ' + nombre)
    cuerpo = comp.bRepBodies.add(temp)
    if cuerpo is None:
        raise RuntimeError('No se pudo guardar cilindro ' + nombre)
    cuerpo.name = nombre


def modelar_todo(app):
    for f in (CAJA, TAPA):
        if not os.path.isfile(f):
            raise RuntimeError('No se encontró el STL: ' + f)
    dis, root = crear_diseno(app)
    root.name = 'Estación Ambiental IoT - Gemelo 3D'
    # Los STL conservan sus coordenadas originales; por eso el origen absoluto
    # definido arriba se suma en cada sólido nuevo.
    cargar_stl(root, CAJA, '01 Caja (STL original)')
    cargar_stl(root, TAPA, '02 Tapa (STL original, separada)', MOV_TAPA)
    ox, oy, oz = ORIGEN
    for nombre, x, y, fondo, tam in PIEZAS:
        largo_x, largo_y, alto_z = tam
        comp = nuevo_componente(root, nombre)
        caja_solida(comp, nombre,
            ox + x, oy + y, oz + fondo + alto_z/2,
            largo_x, largo_y, alto_z)
        if nombre.startswith('06 MQ-9'):
            z1=oz + fondo + alto_z
            cilindro_solido(comp, 'MQ-9 cabeza cilíndrica',
                            ox+x, oy+y, z1, z1+20, 10)
    return dis, 'Ensamble nativo: 2 mallas STL + 5 volúmenes BRep.'


def plan_b(app):
    if not os.path.isfile(ENSAMBLE_3MF):
        raise RuntimeError('No existe el modelo de respaldo .3mf')
    dis, root = crear_diseno(app)
    root.name = 'Estación Ambiental IoT - Gemelo de respaldo 3MF'
    cuerpos = root.meshBodies.add(ENSAMBLE_3MF, adsk.fusion.MeshUnits.MillimeterMeshUnit)
    if cuerpos is None or cuerpos.count < 5:
        raise RuntimeError('Fusion no importó correctamente los cuerpos del 3MF.')
    return dis, 'Modelo de respaldo: {} cuerpos de malla.'.format(cuerpos.count)


def exportar_f3d(dis):
    opts = dis.exportManager.createFusionArchiveExportOptions(SALIDA_F3D)
    if opts is None:
        raise RuntimeError('Fusion no creó las opciones de exportación .f3d')
    if not dis.exportManager.execute(opts):
        raise RuntimeError('Fusion devolvió error al exportar el .f3d')
    if not os.path.isfile(SALIDA_F3D) or os.path.getsize(SALIDA_F3D) < 100:
        raise RuntimeError('La exportación no generó un archivo .f3d válido.')
    return os.path.getsize(SALIDA_F3D)


def run(context):
    app = adsk.core.Application.get()
    ui = app.userInterface
    errores=[]
    try:
        try:
            dis, modo = modelar_todo(app)
        except Exception:
            errores.append('Intento principal: ' + traceback.format_exc())
            dis, modo = plan_b(app)
        try:
            app.activeViewport.fit()
            app.activeViewport.refresh()
        except Exception:
            pass
        try:
            bytes_f3d = exportar_f3d(dis)
            ui.messageBox(
                '{}\n\nF3D GENERADO CORRECTAMENTE:\n{}\nTamaño: {:.1f} MB\n\n'
                'Verifica en pantalla el encaje y las medidas antes de entregar.'.format(
                    modo, SALIDA_F3D, bytes_f3d/1048576),
                'Estación Ambiental IoT - Exportación completada')
        except Exception:
            errores.append('Exportación: ' + traceback.format_exc())
            ui.messageBox(
                'El modelo está abierto en Fusion, pero no se exportó automáticamente.\n'
                'Usa Archivo > Exportar > Fusion 360 Archive (*.f3d).\n\n'
                + '\n'.join(errores)[-4000:],
                'Gemelo creado - Exportación manual necesaria')
    except Exception:
        errores.append('Respaldo: ' + traceback.format_exc())
        ui.messageBox('\n\n'.join(errores)[-5000:], 'Error al crear gemelo 3D')
