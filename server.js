require("dotenv").config();

const express = require("express");
const path = require("path");
const { Pool } = require("pg");

const app = express();

app.use(express.json({ limit: "16kb" }));
app.use(express.static(path.join(__dirname, "public")));

// Conexion a PostgreSQL.
const pool = new Pool({
  connectionString: (process.env.DATABASE_URL || "").split("?")[0],
  ssl: process.env.DB_CA_CERT
    ? { ca: process.env.DB_CA_CERT.replace(/\\n/g, "\n") }
    : { rejectUnauthorized: false },
  max: 5,
  connectionTimeoutMillis: 10000
});

pool.on("error", () => {
  console.error("Error en una conexion inactiva de PostgreSQL");
});

// Prepara la tabla sin borrar datos existentes.
async function crearTabla() {
  await pool.query(`
    CREATE TABLE IF NOT EXISTS lecturas (
      id SERIAL PRIMARY KEY,
      dispositivo VARCHAR(50) NOT NULL,
      sensor VARCHAR(50) NOT NULL,
      valor REAL NOT NULL,
      creado_en TIMESTAMPTZ DEFAULT NOW()
    )
  `);

  console.log("Tabla 'lecturas' lista");
}

// Comprueba la clave antes de permitir guardar una lectura.
function revisarClave(req, res, next) {
  const recibida = req.get("x-api-key") || "";
  const esperada = process.env.API_KEY || "";

  // Diagnostico temporal: no imprime ninguna clave.
  console.log("DIAGNOSTICO API", {
    caracteresRecibidos: recibida.length,
    caracteresEsperados: esperada.length,
    coinciden: recibida === esperada,
    coincideSinEspacios: recibida.trim() === esperada.trim()
  });

  if (!esperada) {
    return res.status(503).json({
      error: "API_KEY no configurada en el servidor"
    });
  }

  if (recibida !== esperada) {
    return res.status(401).json({
      error: "API key invalida"
    });
  }

  next();
}

// Comprueba la conexion con la base de datos.
app.get("/api/salud", async (req, res) => {
  try {
    await pool.query("SELECT 1");
    res.json({ ok: true, db: "conectada" });
  } catch (err) {
    console.error("Error de conexion:", err.code || "sin codigo");
    res.status(500).json({
      ok: false,
      error: "Base no disponible"
    });
  }
});

// Recibe y guarda una lectura del ESP32.
app.post("/api/lecturas", revisarClave, async (req, res) => {
  const { dispositivo, sensor, valor } = req.body || {};

  if (
    typeof dispositivo !== "string" ||
    !dispositivo.trim() ||
    dispositivo.length > 50 ||
    typeof sensor !== "string" ||
    !sensor.trim() ||
    sensor.length > 50 ||
    typeof valor !== "number" ||
    !Number.isFinite(valor)
  ) {
    return res.status(400).json({
      error:
        "Dispositivo y sensor deben ser textos de 1 a 50 caracteres; valor debe ser un numero finito"
    });
  }

  try {
    const resultado = await pool.query(
      `INSERT INTO lecturas (dispositivo, sensor, valor)
       VALUES ($1, $2, $3) RETURNING id`,
      [dispositivo.trim(), sensor.trim(), valor]
    );

    res.status(201).json({
      ok: true,
      id: resultado.rows[0].id
    });
  } catch (err) {
    console.error("Error al guardar:", err.code || "sin codigo");
    res.status(500).json({ error: "Error al guardar" });
  }
});

// Entrega las ultimas lecturas al dashboard.
app.get("/api/lecturas", async (req, res) => {
  const solicitado = Number(req.query.limite ?? 50);

  if (!Number.isInteger(solicitado) || solicitado < 1) {
    return res.status(400).json({
      error: "limite debe ser un entero positivo"
    });
  }

  const limite = Math.min(solicitado, 500);

  try {
    const resultado = await pool.query(
      "SELECT * FROM lecturas ORDER BY id DESC LIMIT $1",
      [limite]
    );

    res.json(resultado.rows);
  } catch (err) {
    console.error("Error al leer:", err.code || "sin codigo");
    res.status(500).json({ error: "Error al leer" });
  }
});

// Railway proporciona PORT; en local se usa 3000.
const PORT = process.env.PORT || 3000;

app.listen(PORT, async () => {
  console.log(`Servidor escuchando en el puerto ${PORT}`);

  try {
    await crearTabla();
  } catch (err) {
    console.error(
      "No se pudo preparar la base de datos:",
      err.code || "revisar configuracion"
    );
  }
});
