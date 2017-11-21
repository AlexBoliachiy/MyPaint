#include "stdafx.h"
#include "Commands.h"

void Commands::StrokeCommand::Execute(CDC * dc)
{
	for (auto& x : stroke)
		x->Draw(dc);
}

void Commands::StrokeCommand::addStroke(Shapes::Line * line)
{
	stroke.push_back(std::shared_ptr<Shapes::Shape>(line));
}

Commands::StrokeCommand::StrokeCommand(std::vector<std::shared_ptr<Shapes::Shape>> stroke)
{
	for (auto& x : stroke)
	{
		stroke.push_back(x);
	}
}

Commands::StrokeCommand::StrokeCommand()
{

}

void Commands::ShapeCommand::Execute(CDC * dc)
{
	shape->Draw(dc);
}

Commands::ShapeCommand::ShapeCommand(Shapes::Shape * shape)
{
	this->shape = std::shared_ptr<Shapes::Shape>(shape);
}

void Commands::NewDocumentCommand::Execute(CDC * dc)
{
	CRect rect;
	AfxGetMainWnd()->GetClientRect(rect);
	dc->FillSolidRect(rect, RGB(255, 255, 255));
}

Commands::OpenDocumentCommand::OpenDocumentCommand(LPCTSTR lpszPathName)
{
	image.Load(lpszPathName);
}

void Commands::OpenDocumentCommand::Execute(CDC * dc)
{
	CRect rect;
	AfxGetMainWnd()->GetClientRect(rect);
	image.BitBlt(*dc, 0, 0, rect.Width(), rect.Height(), 0, 0);
}


const std::vector<std::shared_ptr<Commands::Command>>& Commands::CommandsManager::GetCommands()
{
	return commands;
}

void Commands::CommandsManager::AddCommand(Commands::Command * command)
{
	commands.push_back(std::shared_ptr<Commands::Command>(command));
	thrashBin.clear();
}

void Commands::CommandsManager::Undo()
{
	if (commands.empty())
		return;

	thrashBin.push_back(commands.back());
	commands.pop_back();
}

void Commands::CommandsManager::Redo()
{
	if (thrashBin.empty())
		return;

	commands.push_back(thrashBin.back());
	thrashBin.pop_back();
}
