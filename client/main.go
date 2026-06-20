package main

import (
	"github.com/gofiber/fiber/v3"
	"github.com/gofiber/template/html/v3"
	"github.com/gofiber/fiber/v3/middleware/static"
	"github.com/gofiber/fiber/v3/middleware/session"

	// "fmt"
)

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

				// Get username
					username := sess.Get( "username" )
					if username == nil {
						return c.Redirect().To("/")
					}
					
				// Render chatroom page
					return c.Render("chatroom", fiber.Map{
						"Username": username,
					})
			})

		// POST
			app.Post("/", func(c fiber.Ctx) error {
				username := c.FormValue( "username" )
				sess := session.FromContext(c)
				sess.Set( "username", username )

				return c.Redirect().To("/chatroom")
			})

	app.Listen(":3000")
}