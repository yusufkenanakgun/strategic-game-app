import 'package:flutter/material.dart';
import '../models/game_state.dart';
import '../models/position.dart';

class CellWidget extends StatefulWidget {
  final Position position;
  final CellState state;
  final bool isSelected;
  final bool isValidMove;
  final bool isAIHighlight;
  final VoidCallback onTap;

  const CellWidget({
    super.key,
    required this.position,
    required this.state,
    required this.isSelected,
    required this.isValidMove,
    required this.onTap,
    this.isAIHighlight = false,
  });

  @override
  State<CellWidget> createState() => _CellWidgetState();
}

class _CellWidgetState extends State<CellWidget> {
  bool _isHovered = false;

  @override
  Widget build(BuildContext context) {
    return MouseRegion(
      onEnter: (_) => setState(() => _isHovered = true),
      onExit: (_) => setState(() => _isHovered = false),
      child: GestureDetector(
        onTap: widget.onTap,
        child: AnimatedContainer(
          duration: const Duration(milliseconds: 200),
          curve: Curves.easeInOut,
          decoration: BoxDecoration(
            color: _getCellColor(),
            border: Border.all(
              color: widget.isSelected
                  ? Colors.amber
                  : _isHovered && widget.state != CellState.removed
                      ? Colors.grey[400]!
                      : Colors.grey[300]!,
              width: widget.isSelected ? 3 : (_isHovered ? 2 : 1),
            ),
            borderRadius: BorderRadius.circular(8),
            boxShadow: _isHovered && widget.state != CellState.removed
                ? [
                    BoxShadow(
                      color: Colors.black.withOpacity(0.1),
                      blurRadius: 8,
                      offset: const Offset(0, 2),
                    ),
                  ]
                : null,
          ),
          transform: _isHovered && widget.state != CellState.removed
              ? (Matrix4.identity()..scale(1.05))
              : Matrix4.identity(),
          child: Center(
            child: _getCellContent(),
          ),
        ),
      ),
    );
  }

  Color _getCellColor() {
    if (widget.state == CellState.removed) {
      return const Color(0xFF37474F);
    }

    if (widget.isAIHighlight) {
      return Colors.orange[200]!;
    }

    if (widget.isValidMove) {
      return _isHovered ? Colors.green[200]! : Colors.green[100]!;
    }

    if (widget.isSelected) {
      return Colors.amber[100]!;
    }

    if (_isHovered) {
      return Colors.grey[200]!;
    }

    return const Color(0xFFFAFAFA);
  }

  Widget _getCellContent() {
    switch (widget.state) {
      case CellState.player1:
        return TweenAnimationBuilder<double>(
          tween: Tween(begin: 0.0, end: 1.0),
          duration: const Duration(milliseconds: 300),
          builder: (context, value, child) {
            return Transform.scale(
              scale: value,
              child: Container(
                width: 45,
                height: 45,
                decoration: BoxDecoration(
                  gradient: LinearGradient(
                    colors: [Colors.blue[400]!, Colors.blue[600]!],
                    begin: Alignment.topLeft,
                    end: Alignment.bottomRight,
                  ),
                  shape: BoxShape.circle,
                  boxShadow: [
                    BoxShadow(
                      color: Colors.blue.withOpacity(0.4),
                      blurRadius: 8,
                      offset: const Offset(0, 2),
                    ),
                  ],
                ),
                child: const Center(
                  child: Text(
                    'AI',
                    style: TextStyle(
                      color: Colors.white,
                      fontWeight: FontWeight.bold,
                      fontSize: 14,
                      letterSpacing: 1,
                    ),
                  ),
                ),
              ),
            );
          },
        );

      case CellState.player2:
        return TweenAnimationBuilder<double>(
          tween: Tween(begin: 0.0, end: 1.0),
          duration: const Duration(milliseconds: 300),
          builder: (context, value, child) {
            return Transform.scale(
              scale: value,
              child: Container(
                width: 45,
                height: 45,
                decoration: BoxDecoration(
                  gradient: LinearGradient(
                    colors: [Colors.red[400]!, Colors.red[600]!],
                    begin: Alignment.topLeft,
                    end: Alignment.bottomRight,
                  ),
                  shape: BoxShape.circle,
                  boxShadow: [
                    BoxShadow(
                      color: Colors.red.withOpacity(0.4),
                      blurRadius: 8,
                      offset: const Offset(0, 2),
                    ),
                  ],
                ),
                child: const Center(
                  child: Text(
                    'YOU',
                    style: TextStyle(
                      color: Colors.white,
                      fontWeight: FontWeight.bold,
                      fontSize: 11,
                      letterSpacing: 0.8,
                    ),
                  ),
                ),
              ),
            );
          },
        );

      case CellState.removed:
        return Icon(
          Icons.block,
          color: Colors.white.withOpacity(0.5),
          size: 30,
        );

      case CellState.empty:
        if (widget.isValidMove) {
          return AnimatedContainer(
            duration: const Duration(milliseconds: 300),
            width: _isHovered ? 28 : 24,
            height: _isHovered ? 28 : 24,
            decoration: BoxDecoration(
              color: Colors.green.withOpacity(0.3),
              shape: BoxShape.circle,
              border: Border.all(
                color: Colors.green,
                width: 2,
              ),
            ),
          );
        }
        return const SizedBox.shrink();
    }
  }
}
