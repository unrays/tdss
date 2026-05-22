# EXOTIC.tdss

A C++ data-oriented registry with compile-time type mapping and cache-efficient handle-based storage.
Originally designed as the data layer of a compiler architecture, this system is general-purpose and can be used in other data-oriented systems.

## Features

- Compile-time type-to-storage mapping
- Contiguous cache-friendly storage
- Handle-based object access
- Data-oriented architecture
- Minimal runtime overhead

## Storage Model

The underlying storage system is built around a hybrid stack/heap strategy.

Each `SmartStorage<T, N>` is statically evaluated at compile time:

- If the total storage size fits within a fixed internal threshold, data is allocated on the stack.
- Otherwise, it falls back to heap allocation with aligned memory.

This allows predictable performance for small datasets while maintaining scalability for larger ones.

## Configuration

```cpp
using NodeDataRegistryTable = LinearTable<
    Entry<NodeInstruction,         InstructionNodeData>,
    Entry<NodeCallFunction,        FunctionCallNodeData>,
    Entry<NodeDeclarationFunction, FunctionDeclarationNodeData>,
    Entry<NodeLiteral,             LiteralNodeData>,
    Entry<NodeOperation,           OperationNodeData>
>;

using NodeDataRegistry = MultiStorageRegistry<NodeDataRegistryTable, NodeHandleProvider, 1 << 20>;

NodeDataRegistry registry(provider);
```

The `MultiStorageRegistry<Table, Handle, N>` is a facade that aggregates multiple data domains, driven by a compile-time configuration table, into a unified internal allocation system built on top of `SmartStorage<T, N>` previously introduced. Additionally, it is possible to define a specific size for `MultiStorageRegistry<..., N>` in bytes, which is then evenly distributed across the underlying storage subsystems.

## Usage

```cpp
registry.construct(nodeLiteral, token);
auto& literalData = registry.get(nodeLiteral);

registry.construct_for<LiteralNodeData>(nodeLiteral, token);

registry.construct(nodeOperation, lhs, rhs);
auto& operationData = registry.get(nodeOperation);

registry.reset();
```

## License

This project is licensed under the Boost Software License. See the [LICENSE](LICENSE) file for details.

<p align="center"><sub>© Félix-Olivier Dumas 2026</sub></p>
