#pragma once
#include "Shapes.h"

namespace Commands
{
	//Paint stroke command
	class Command
	{
	public:
		virtual void Execute(CDC* dc) = 0;
	};

	class StrokeCommand : public Command
	{
	private:
		std::vector<std::shared_ptr<Shapes::Shape>> stroke;
	public:
		virtual void Execute(CDC *dc) override;
		void addStroke(Shapes::Line * line);
		StrokeCommand(std::vector<std::shared_ptr<Shapes::Shape>> stroke);
		StrokeCommand();
	};


	// Paint shape command
	class ShapeCommand : public Command
	{
	private:
		std::shared_ptr<Shapes::Shape> shape;

	public:
		virtual void Execute(CDC *dc) override;
		ShapeCommand(Shapes::Shape* shape);

	};


	class NewDocumentCommand : public Command
	{

	public:
		virtual void Execute(CDC* dc) override;
	};



	class OpenDocumentCommand : public Command
	{
	private:
		CImage image;
	public:
		OpenDocumentCommand(LPCTSTR lpszPathName);
		virtual void Execute(CDC* dc) override;
	};

	class CommandsManager
	{
	private:
		std::vector<std::shared_ptr<Commands::Command>> commands;
		std::vector<std::shared_ptr<Commands::Command>> thrashBin;
	public:
		const std::vector<std::shared_ptr<Commands::Command>>& GetCommands();
		void AddCommand(Commands::Command * command);
		void Undo();
		void Redo();


	};
}
