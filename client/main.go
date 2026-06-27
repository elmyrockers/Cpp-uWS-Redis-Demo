package main

import (
	"os"
	"time"

	"github.com/gofiber/fiber/v3"
	"github.com/gofiber/template/html/v3"
	"github.com/gofiber/fiber/v3/middleware/static"
	"github.com/gofiber/fiber/v3/middleware/session"
	"github.com/golang-jwt/jwt/v5"
)

func generateJWTToken( username string ) (string, error) {
	// Load private key
		privateKeyData, err := os.ReadFile("keys/private.pem")
		if err != nil {
			return "", err
		}

	// Parse private key
		signingKey, err := jwt.ParseRSAPrivateKeyFromPEM(privateKeyData)
		if err != nil {
			return "", err
		}

	// Claims with 1-hour expiry
		claims := jwt.MapClaims{
			"username": username,
			"exp":      time.Now().Add(time.Hour).Unix(),
		}

	// Create and sign token
		token := jwt.NewWithClaims(jwt.SigningMethodRS256, claims)
		signedToken, err := token.SignedString(signingKey)
		if err != nil {
			return "", err
		}

	return signedToken, nil
}

func main() {
	// Use `html/template` view engine
		engine := html.New("./views", ".html")
		app := fiber.New(fiber.Config{
			Views: engine,
		})

	// Set session
		app.Use(session.New())

	// Route for static files
		app.Use("/", static.New("./public"))

	// Routes for view
		// GET
			app.Get("/", func(c fiber.Ctx) error {
				return c.Render("index", fiber.Map{})
			})
			app.Get("/chatroom", func(c fiber.Ctx) error {
				sess := session.FromContext(c)

				// Get username from session
					username := sess.Get( "username" )
					if username == nil {
						return c.Redirect().To("/")
					}

				// Generate JWT token to be used with websocket auth
					token := "";

				// Render chatroom page
					return c.Render("chatroom", fiber.Map{
						"Username": username,
						"Token": token,
					})
			})

		// POST
			app.Post("/", func(c fiber.Ctx) error {
				// Store username into session
					username := c.FormValue( "username" )
					sess := session.FromContext(c)
					sess.Set( "username", username )

				// Redirect to chatroom page
					return c.Redirect().To("/chatroom")
			})

	app.Listen(":3000")
}